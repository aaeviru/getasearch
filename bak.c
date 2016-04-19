#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <geta/dtypes.h>
#include <geta/dw.h>
#include <geta/dwsched.h>
#include <geta/dxr.h>
#include <geta/dwwt.h>
#include <geta/dwsh.h>
#include <geta/dwam.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>
#define RESULTPATH "/mnt/nas2a/ko/rbak/"
int trave_dir(char* path,char** filename)
{
    DIR *d; //声明一个句柄
    struct dirent *file; //readdir函数的返回值就存放在这个结构体中
    struct stat sb;   
    int i =0;
    if(!(d = opendir(path)))
    {
        printf("error opendir %s!!!\n",path);
        return -1;
    }
    while((file = readdir(d)) != NULL)
    {
        if(strncmp(file->d_name, ".", 1) == 0)
            continue;
	if(file->d_name[strlen(file->d_name)-1] == 't'){	
		filename[i] = malloc( (strlen(path)+strlen(file->d_name) + 2 )*sizeof(path[0]) );
		strcpy(filename[i],path);
		strcat(filename[i],file->d_name);
		i++;
	}
    }
    closedir(d);
    printf("tarve complete\n");
    return i;
}
int query(struct syminfo* q, WAM* w, char** a, ssize_t* lenth, int type, char** sw, int sw_lenth){
	int i,j,sw_check = 0;
	int len = 0;
	u_int id;
	printf("%lld\n",*lenth);
	for(i=0;i<*lenth;i++){
		for(j=0;j<sw_lenth;j++){
			if(strcmp(sw[j],a[i]) == 0){
				sw_check = 1;
				break;
			}
		}
		if (sw_check == 1){
			sw_check = 0;
			free(a[i]);
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
		free(a[i]);
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
	FILE *fp;
	wam_init(NULL);
	WAM *w;
	u_int id;
        char *name;
	w = wam_open(argv[1]);
/*
	if (!(id = wam_name2id(w, WAM_COL, name))) {        
                fprintf(stderr, "この 単語 は登録されていません: %s\n", name);
        }
	
	printf("id:%d\n",id);

	if (!(name = wam_id2name(w, WAM_COL, id))) {        
                fprintf(stderr, "この id は登録されていません: %d\n", id);
        }
	printf("name:%s\n",name);
	struct xr_vec *vec;
        u_int elem_num;
        if ((elem_num = wam_get_vec(w, WAM_COL, id, &vec))==-1) {
                err(1, "失敗: get_vec");
        }
*/
	int i,j;
/*
        for (i=0; i<elem_num; i++) {
		struct xr_elem *e = &vec->elems[i];
                char *name;
                name = wam_id2name(w, WAM_ROW, e->id);
                printf("id=%d, name=%s, frq=%d\n", e->id, name, e->freq);
        }
*/	


	struct syminfo qq[1000];
	struct syminfo *q = qq;
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
	for(j=0;j<filenum;j++){
		k = 0;
		fp = fopen(filename[j],"r");
		printf("%s\n",filename[j]);
		term[k] = malloc( 100*sizeof(char) );
		while(fscanf(fp,"%s",term[k]) != EOF){ 
			k++;
			term[k] = malloc( 100*sizeof(char) );
		}
		free(term[k]);

		fclose(fp);
		char opfile[50];
		snprintf(opfile,50,"%s%s",RESULTPATH,filename[j]+strlen(filename[j])-8);
		//printf("file:%s/n",opfile);
		fp = fopen(opfile,"w");
		query(q,w,term,&k,1,sw,sw_len);
		ssize_t len = 0;
        	ssize_t num = 1000;
        	if(k <= 0){
                	fprintf(stderr, "qlen <= 0\n");
                	return -1;
        	}

        	struct syminfo *result = wsh(q,k,w,WAM_COL,WT_SMARTAW,&num,&len);
		if(len > 1000) printf("over 1000\n");
        	fprintf(fp,"query:%d\nnum:%d\nresult:%d\n",k,num,len);
        	for(i=0;i<num;i++){
                	fprintf(fp,"name:%s df_d:%d df:%d tf_d:%d tf:%d weight:%lf\n",wam_id2name(w, WAM_ROW, result[i].id),result[i].DF_d,result[i].DF,result[i].TF_d,result[i].TF,result[i].weight);
		}
		fclose(fp);
	
	}
/*
	ssize_t len = 0;
	ssize_t num = 10;
	if(lenth <= 0){
		fprintf(stderr, "qlen <= 0\n");
		return -1;
	}
	struct syminfo *result = wsh(q,lenth,w,WAM_COL,WT_SMARTAW,&num,&len);
	printf("%d\n",len);
	for(i=0;i<num;i++){
		printf("name:%s df_d:%d df:%d tf_d:%d tf:%d weight:%lf\n",wam_id2name(w, WAM_ROW, result[i].id),result[i].DF_d,result[i].DF,result[i].TF_d,result[i].TF,result[i].weight);
//	wsh(&q,1,w,WAM_COL,WT_SMART,&num,&len,NULL,compar_syminfo_weight);
	}
*/
	return 0;
}
