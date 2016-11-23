#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <geta/dtypes.h>
#include <geta/dw.h>
#include <geta/dwsched.h>
#include <geta/dxr.h>
#include <geta/dwwt.h>
#include <geta/dwsh.h>
#include <geta/dwam.h>
#include <math.h>
#include <assert.h>
#define NaN (0.0/0.0)
int main(int argc,char *argv[]){
	if(argc < 2){
		printf("input:wam query\n");
		return 1;
	}
	FILE *fp;
	wam_init(NULL);
	WAM *w;
	u_int id;
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
/*
        for (i=0; i<elem_num; i++) {
		struct xr_elem *e = &vec->elems[i];
                char *name;
                name = wam_id2name(w, WAM_ROW, e->id);
                printf("id=%d, name=%s, frq=%d\n", e->id, name, e->freq);
        }
*/	
	ssize_t lenth = argc -2;
	int i = 2,j = 0;
	struct syminfo q[10];
	printf("lenth:%lld\n",lenth);
	if(q){
		while(i<argc){
			int type = 0;
			if(argv[i][0] == '-'){
	                	type = argv[i][1] - '0';
				i++;
				lenth--;
        		}
			if (!(id = wam_name2id(w, WAM_COL, argv[i]))) {        /* 列: ID -> name */
		                fprintf(stderr, "この 単語 は登録されていません: %s\n", argv[2+i]);
				return -1;
        		}
			printf("name:%s id:%lld\n",argv[i],id);
			q[j].id = id;
        		q[j].weight = NaN;
			switch(type){
				case 0:
        				q[j].attr = WSH_OR;
					break;
				case 1:
                                        q[j].attr = WSH_AND;
                                        break;
				case 2:
                                        q[j].attr = WSH_NOT;
                                        break;
				default:
					q[j].attr = WSH_OR;
                                        break;
			}
			i++;
			j++;
		}

	}else{
	}
/*
	q.id = id;
	q.TF_d = 1;
	q.TF = 1;
	q.weight = NaN;
	q.attr = WSH_OR;
*/
	ssize_t len = 0;
	ssize_t num = 10;
	struct syminfo *result = wsh(q,lenth,w,WAM_COL,WT_SMARTAW,&num,&len);
	printf("%d\n",len);
	for(i=0;i<num;i++){
		char name[10];
		snprintf(name,9,"%s",wam_id2name(w, WAM_ROW, result[i].id)); 
		printf("name:%s df_d:%d df:%d tf_d:%d tf:%d weight:%lf\n",name,result[i].DF_d,result[i].DF,result[i].TF_d,result[i].TF,result[i].weight);
//	wsh(&q,1,w,WAM_COL,WT_SMART,&num,&len,NULL,compar_syminfo_weight);
	}

	wam_close(w);
	return 0;
}
