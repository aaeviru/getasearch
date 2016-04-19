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
#define RESULTPATH "/mnt/nas2a/ko/split/"
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
	if ( argc < 3 ){
		printf("input error:handle splitnum\n");
		return -1;
	}
	FILE *fp;
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
	int l;
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
		if ( strcmp(argv[2] , "all" ) == 0 ){
                        snprintf(opfile,50,"%s/split/%s",RESULTPATH,filename[j]+strlen(filename[j])-8);
                        fp = fopen(opfile,"w");
                        for(l=0;l<k;l++){
				ssize_t kk = 1;
                                query(q,w,&term[l],&kk,0,sw,sw_len);
				if(kk <= 0){
                                        continue;
                                }

                                ssize_t len = 0;
                                ssize_t num = 1000;

                                struct syminfo *result = wsh(q,kk,w,WAM_COL,WT_SMARTAW,&num,&len);
                                fprintf(fp,"query:%d\nnum:%d\nresult:%d\n",kk,num,len);
                                for(i=0;i<num;i++){
                                        fprintf(fp,"name:%s df_d:%d df:%d tf_d:%d tf:%d weight:%lf\n",wam_id2name(w, WAM_ROW, result[i].id),result[i].DF_d,result[i].DF,result[i].TF_d,result[i].TF,result[i].weight);
                                }
                        }
                        fclose(fp);
		}else{
			int SPLIT = atoi(argv[2]);
			snprintf(opfile,50,"%s/%d/%s",RESULTPATH,SPLIT,filename[j]+strlen(filename[j])-8);
			//printf("file:%s\n",opfile);
			fp = fopen(opfile,"w");
			for(l=0;l<SPLIT;l++){
				ssize_t kkk = k/SPLIT;
				ssize_t kk = kkk;
				if(l == SPLIT-1) kk = k - kkk*l;
				query(q,w,&term[kkk*l],&kk,0,sw,sw_len);
				ssize_t len = 0;
        			ssize_t num = 1000;
        			if(kk <= 0){
                			printf("qlen <= 0\n");
					fprintf(fp,"query:%d\nnum:%d\nresult:%d\n",kk,0,len);
					continue;
        			}
	
        			struct syminfo *result = wsh(q,kk,w,WAM_COL,WT_SMARTAW,&num,&len);
				if(len > 1000) printf("%s over 1000\n",filename[j]);
        			fprintf(fp,"query:%d\nnum:%d\nresult:%d\n",kk,num,len);
        			for(i=0;i<num;i++){
                			fprintf(fp,"name:%s df_d:%d df:%d tf_d:%d tf:%d weight:%lf\n",wam_id2name(w, WAM_ROW, result[i].id),result[i].DF_d,result[i].DF,result[i].TF_d,result[i].TF,result[i].weight);
				}	
			}
			fclose(fp);
		}
	
	}
	return 0;
}
