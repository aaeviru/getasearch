#include "getapy.h"

WAM* init(char* wam){
	wam_init(NULL);
	return wam_open(wam);
}

void fin(WAM* w){
	wam_close(w);
}


int search(WAM* w,int argc,char* argv[],int* r,int maxnum){
	FILE *fp;
	u_int id;
	ssize_t lenth = argc;
	int i = 0,j = 0;
	struct syminfo q[100];
	//printf("lenth:%lld\n",lenth);
	if(q){
		while(i<argc){
			int type = 0;
			if(argv[i][0] == '-'){
	         type = argv[i][1] - '0';
				i++;
				lenth--;
        	}
			if (!(id = wam_name2id(w, WAM_COL, argv[i]))) {       
		      fprintf(stderr, "この 単語 は登録されていません: %s\n", argv[2+i]);
				return -1;
        	}
			//printf("name:%s id:%lld\n",argv[i],id);
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
	ssize_t len = 0;
	ssize_t num = maxnum;
	struct syminfo *result = wsh(q,lenth,w,WAM_COL,WT_SMART,&num,&len);
	if (len < num)num = len;
	//printf("%d\n",len);

	for(i=0;i<num;i++){
		char name[10];
		snprintf(name,9,"%s",wam_id2name(w, WAM_ROW, result[i].id)); 
		r[i] = atoi(name);
		//printf("in:%s\n",name);
	}

	return num;
}
