#include <stdio.h>
#include <string.h>
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
int query(WAM* w,char* tmpt){
	u_int id;
	ssize_t lenth = 0;
   struct syminfo q[1000];
	char * pch;
	pch = strtok(tmpt," ");
	while(pch != NULL){
		if (!(id = wam_name2id(w, WAM_COL, pch))) {        /* 列: ID -> name */
			pch = strtok(NULL," ");
			continue;
      }
      q[lenth].id = id;
      q[lenth].weight = NaN;
		q[lenth].attr = WSH_OR;
		pch = strtok(NULL," ");
		lenth++;
	}
  	ssize_t len = 0;
   ssize_t num = 1; 
   struct syminfo *result = wsh(q,lenth,w,WAM_COL,WT_SMARTAW,&num,&len);
	return len;
}
int main(int argc,char *argv[]){
	FILE *fp;
	wam_init(NULL);
	WAM *w;
	w = wam_open(argv[1]);
	FILE* fin = fopen(argv[2],"r");
	char tmpt[50000];
	char tmpq[100000];
	int index = 0;
	double perc = 0;
	while(fgets(tmpt,50000,fin) != NULL){
		if(strlen(tmpt) > 1)
			tmpt[strlen(tmpt)-1] = '\0';
		else
			continue;
		if(tmpt[0] == '@'){
			memset(tmpq,0,sizeof(tmpq));
			index = 0;
			printf("%s\n",tmpt);
		}
		else if(tmpt[0] == '!'){
			perc += 1.0 * query(w,tmpq) / query(w,&tmpt[1]);
		}
		else{
			strcpy(&tmpq[index],tmpt);
			index += strlen(tmpt);
			tmpq[index++] = ' ';
		}
	}
	fclose(fin);
	printf("%lf\n",perc);
	wam_close(w);
	return 0;
}
