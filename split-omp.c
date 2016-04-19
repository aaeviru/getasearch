#include "split-omp.h"
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
int main(int argc,char *argv[]){
	int TESTMAX;
	if ( argc < 3 ){
		printf("input error:handle splitnum\n");
		return -1;
	}
	if (argc > 3 ) 	TESTMAX = atoi(argv[3]);
	else		TESTMAX = 1000;
	FILE *fp = NULL;
	wam_init(NULL);
	WAM *w;
	u_int id;
        char *name;
	w = wam_open(argv[1]);
	if(!w){
		printf("wamopen error\n");
		return -1;
	}
	int i,j;


	//struct syminfo qq[1000];
	//struct syminfo *q = qq;
/*
	q.id = id;
	q.TF_d = 1;
	q.TF = 1;
	q.weight = NaN;
	q.attr = WSH_OR;
*/
	char ** sw = malloc(100 * sizeof( char* ) );;
	int sw_len = sw_init(sw);
	char** filename = malloc(4000 * sizeof(char*) );
	int filenum = trave_dir("/home/ko/topics/",filename);
	char temp[100] , *p, *term[1000];
	ssize_t k = 0;
	int l;
	int score = 0;
	for(j=0;j<filenum;j++){
		k = 0;
		fp = fopen(filename[j],"r");
		printf("%s\n",filename[j]);
		term[k] = malloc( 100*sizeof(char) );
		while(fscanf(fp,"%s",term[k]) != EOF){ 
			k++;
			term[k] = NULL;
			term[k] = malloc( 100*sizeof(char) );
		}
		if(term[k] != NULL) free(term[k]);

		if(fp != NULL){
			fclose(fp);
			fp = NULL;
		}
		char opfile[50];
		if ( strcmp(argv[2] , "all" ) == 0 ){

                        //snprintf(opfile,50,"%s/split/%s",RESULTPATH,filename[j]+strlen(filename[j])-8);
                        //fp = fopen(opfile,"w");
			struct list** ll;
			int count = 0;
                        if( NULL == ( ll = malloc(k * sizeof(struct list*)) ) ){
                                return -1;
                        }
                        for(i=0;i<k;i++){
                                if( NULL == (ll[i] = malloc(sizeof(struct list))) ){
                                        return -1;
                                }
				newlist(ll[i],1000);
                        }
			

#pragma omp parallel for private (l) reduction (+:count)
                        for(l=0;l<k;l++){
				ssize_t kk = 1;
				struct syminfo qq[1000];
        			struct syminfo *q = qq;
                                query(q,w,&term[l],&kk,0,sw,sw_len);
				if(kk <= 0){
                                        continue;
                                }
				struct list* splitl = ll[l];
				count += 1;
                                ssize_t len = 0;
                                ssize_t num = 1000;

                                struct syminfo *result = wsh(q,kk,w,WAM_COL,WT_SMARTAW,&num,&len);
				for(i=0;i<num;i++){
                                        char tmpnum[9];
                                        snprintf(tmpnum,9,"%s",wam_id2name(w, WAM_ROW, result[i].id));
                                        list_add(splitl,atoi(tmpnum));
                                }

                        }
			struct list listall;
			int SPLIT = k;
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

                        }
                        printf("search completed:%d\n",listall.num);
                        listscore(&listall,SPLIT,CONST_C);
                        listsort(&listall);
                        //printlist(&listall);
                        snprintf(opfile,50,"%s%s",CHECKPATH,filename[j]+strlen(filename[j])-8,l);
                        printf("%s\n",opfile);
                        fp = fopen(opfile,"r");
                        int maxnum = listall.num<1000 ? listall.num:1000;
                        int hit = 0,total = 0;
                        char buff[1000];
                        while( fgets(buff,1000,fp) != NULL){
                                if(buff[4] != ':') continue;
                                total++;
                                char tmpnum[9];
                                snprintf(tmpnum,9,"%s",&buff[5]);
                                for(i=0;i<maxnum;i++){
                                        if(atoi(tmpnum) == listall.n[i].num) hit++;
                                }
                        }
 			freelist(&listall);
                        printf("%d\n",hit);
                        int zz;
                        //scanf("%d",&zz);
                        fclose(fp);
			return 0;

		}else{
			int SPLIT = atoi(argv[2]);
			//snprintf(opfile,50,"%s/%d/%s",RESULTPATH,SPLIT,filename[j]+strlen(filename[j])-8);
			//fp = fopen(opfile,"w");
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
			
#pragma omp parallel for private (l)
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
					list_add(ll[l],atoi(tmpnum));
				}
			}
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
			printf("search completed:%d\n",listall.num);
			listscore(&listall,SPLIT,CONST_C);
			listsort(&listall);
			//printlist(&listall);
			snprintf(opfile,50,"%s%s",CHECKPATH,filename[j]+strlen(filename[j])-8,l);
			fp = fopen(opfile,"r");
			int maxnum = listall.num<1000 ? listall.num:1000;
			int hit = 0,total = 0;
			char buff[1000];
			while( fgets(buff,1000,fp) != NULL){
				if(buff[4] != ':') continue;
				total++;
				char tmpnum[9];
				snprintf(tmpnum,9,"%s",&buff[5]);
				for(i=0;i<maxnum;i++){
					if(atoi(tmpnum) == listall.n[i].num) hit++;
				}
				if(total >= TESTMAX) break;
			}
			if(listall.n != NULL) freelist(&listall);
			score += hit;
			printf("%d\n",hit);
			//int zz;
			//scanf("%d",&zz);
			fclose(fp);
		}
	
	}
	printf("%d\n",score);
	return 0;
}
