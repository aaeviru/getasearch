#include "split-fusion.h"
#define RESULTPATH "/mnt/nas2a/ko/omp/"
#define CHECKPATH "/mnt/nas2a/ko/result/or/"
#define CONST_C 1.0 / ( 60 + 1000 * 2)
int query(struct syminfo* q, WAM* w, char** a, ssize_t* lenth, int type, char** sw, int sw_lenth){
	int i,j,sw_check = 0;
	int len = 0;
	u_int id;
	for(i=0;i<*lenth;i++){
		for(j=0;j<sw_lenth;j++){
			if(strcmp(sw[j],a[i]) == 0){
				sw_check = 1;
				break;
			}
		}
		if (sw_check == 1){
			sw_check = 0;
			if(a[i] != NULL){  free(a[i]); a[i] = NULL;}
			continue;
		}
        	if (!(id = wam_name2id(w, WAM_COL, a[i]))) continue;
                q[len].id = id;
                q[len].weight = 0.0/0.0;
		switch(type){
		case 0:
                	q[len].attr = WSH_OR;
			break;
		case 1:
			q[len].attr = WSH_AND;
			break;
		default:
			break;
		}
		len++;
		if(a[i] != NULL){  free(a[i]); a[i] = NULL;}
        }
	*lenth = len;
	return 0;

}
int sw_init(char** sw){
	FILE *fp;
	fp = fopen("/mnt/nas2a/ko/stopword.txt","r");
	int num,i = 0;
	char name[50];
	while(fscanf(fp,"%s %d\n",name,&num) != EOF){
		sw[i] = malloc(strlen(name)*sizeof(name[0]));
		strcpy(sw[i],name);
		i++;
	}
	fclose(fp);
	printf("sw_init completed\n");
	return i;
}

int fusion(struct hitnode* rh,char* filename,struct list** ll,int SPLIT,int TESTMAX){
	FILE *fp;
	int i,k,l;
	struct list listall;
        newlist(&listall,1000*SPLIT);
        for(l=0;l<SPLIT;l++){
                for(i=0;i<ll[l]->num;i++){
                        for(k=0;k<listall.num;k++){
                                if(ll[l]->n[i].num == listall.n[k].num){
                                        listall.n[k].score += ll[l]->n[i].score;
                                        listall.n[k].count ++;
                                        break;
                                }
                        }
                        if(k == listall.num) list_add2(&listall,ll[l]->n[i].num,ll[l]->n[i].score);
                }
                        if(ll[l]->n != NULL) freelist(ll[l]);
                        if(ll[l]){ free(ll[l]); ll[l] = NULL;}
        }
        if(ll){ free(ll); ll=NULL;}
        //printf("search completed:%d\n",listall.num);
        listscore(&listall,SPLIT,CONST_C);
        listsort(&listall);
        //printlist(&listall);
        char opfile[50];
        snprintf(opfile,50,"%s%s",CHECKPATH,filename+strlen(filename)-8,l);
        fp = NULL;
        fp = fopen(opfile,"r");
        int maxnum = listall.num<1000 ? listall.num:1000;
        int hit = 0,total = 0,hitinf = 0,hitinflT;
        char buff[1000];
        while( fgets(buff,1000,fp) != NULL){
                if(buff[4] != ':') continue;
                total++;
                char tmpnum[9];
                snprintf(tmpnum,9,"%s",&buff[5]);
                for(i=0;i<listall.num;i++){
                        if(atoi(tmpnum) == listall.n[i].num){
				hit++;
				if( i < maxnum) hitinf++;
				break;
			}
                }
		//if( i == listall.num)  printf("%s\n",tmpnum);
                if(total == TESTMAX) hitinflT = hitinf;
        }
        if(listall.n != NULL) freelist(&listall);
        //printf("%d\n",hit);
        //int zz;
        //scanf("%d",&zz);
        fclose(fp);
	rh->hit = hit;rh->hitinf = hitinf;rh->total = listall.num;rh->hitinflT = hitinflT;
	return 0;
}
int splitsearch(struct hitnode* rh,WAM* w,char* filename,int SPLIT,char** sw,int sw_len,int TESTMAX,int KK){
	char *term[1000];
        ssize_t k = 0;
	FILE* fp = NULL;
        fp = fopen(filename,"r");
        //printf("%s\n",filename);
        term[k] = malloc( 100*sizeof(char) );
        while(fscanf(fp,"%s",term[k]) != EOF){
        	k++;
                term[k] = NULL;
                term[k] = malloc( 100*sizeof(char) );
        }
        if(term[k] != NULL) free(term[k]);
        fclose(fp);
	int i,j,l;
	struct list** ll = NULL;
	if( NULL == ( ll = malloc(SPLIT * sizeof(struct list*)) ) ){
		return -1;
	}
	for(i=0;i<SPLIT;i++){
		ll[i] = NULL;
		if( NULL == (ll[i] = malloc(sizeof(struct list))) ){
			return -1;
		}
		newlist(ll[i],1000);
	}
	for(l=0;l<SPLIT;l++){
		ssize_t kkk = k/SPLIT;
		ssize_t kk = kkk;
		if(l == SPLIT-1) kk = k - kkk*l;
		struct syminfo qq[1000];
       		struct syminfo *q = qq;
		query(q,w,&term[kkk*l],&kk,0,sw,sw_len);
		ssize_t len = 0;
       		ssize_t num = 1000;
       		if(kk <= 0){
			continue;
       		}
       		struct syminfo *result = wsh(q,kk,w,WAM_COL,WT_SMARTAW,&num,&len);
       		for(i=0;i<num;i++){
			char tmpnum[9];
       			snprintf(tmpnum,9,"%s",wam_id2name(w, WAM_ROW, result[i].id));
			list_add(ll[l],atoi(tmpnum),KK);
		}
	}
	return fusion(rh,filename,ll,SPLIT,TESTMAX);
}
	
int splitall(struct hitnode* rh,WAM* w,char* filename,int TERMLEN,char** sw,int sw_len,int TESTMAX,int KK){
        char *term[1000];
        ssize_t k = 0;
        FILE* fp = NULL;
        fp = fopen(filename,"r");
        //printf("%s\n",filename);
        term[k] = malloc( 100*sizeof(char) );
        while(fscanf(fp,"%s",term[k]) != EOF){
                k++;
                term[k] = NULL;
                term[k] = malloc( 100*sizeof(char) );
        }
        if(term[k] != NULL) free(term[k]);
        fclose(fp);
        int i,j,l;
        struct list** ll = NULL;
	int SPLIT = (k+TERMLEN-1) / TERMLEN;
        if( NULL == ( ll = malloc(SPLIT * sizeof(struct list*)) ) ){
                return -1;
        }
        for(i=0;i<SPLIT;i++){
                ll[i] = NULL;
                if( NULL == (ll[i] = malloc(sizeof(struct list))) ){
                        return -1;
                }
                newlist(ll[i],1000);
        }
        for(l=0;l<SPLIT;l++){
                ssize_t kkk = k/SPLIT;
                ssize_t kk = kkk;
                if(l == SPLIT-1) kk = k - kkk*l;
                struct syminfo qq[1000];
                struct syminfo *q = qq;
                query(q,w,&term[kkk*l],&kk,0,sw,sw_len);
                ssize_t len = 0;
                ssize_t num = 1000;
                if(kk <= 0){
                        continue;
                }
                struct syminfo *result = wsh(q,kk,w,WAM_COL,WT_SMARTAW,&num,&len);
                for(i=0;i<num;i++){
                        char tmpnum[9];
                        snprintf(tmpnum,9,"%s",wam_id2name(w, WAM_ROW, result[i].id));
                        list_add(ll[l],atoi(tmpnum),KK);
                }
        }
        return fusion(rh,filename,ll,SPLIT,TESTMAX);
}

