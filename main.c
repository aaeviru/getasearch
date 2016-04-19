#include "split-fusion.h"
#define RESULTPATH "/mnt/nas2a/ko/omp/"
#define CHECKPATH "/mnt/nas2a/ko/result/or/"
#define CONST_C 1.0 / ( 60 + 1000 * 2)

int main(int argc,char *argv[]){
	int TESTMAX;
	if ( argc < 3 ){
		printf("input error:handle splitnum\n");
		return -1;
	}
	if (argc > 3 ) 	TESTMAX = atoi(argv[3]);
	else		TESTMAX = 1000;
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
	int i,j,KK;
	struct hitnode rh;
	char ** sw = malloc(100 * sizeof( char* ) );;
	int sw_len = sw_init(sw);
	char** filename = malloc(4000 * sizeof(char*) );
	int filenum = trave_dir("/home/ko/topics/",filename);
	int score = 0,totalnum = 0,recall = 0,scorelT = 0,SPLIT = 0;
	for(j=0;j<filenum;j++){
		if ( strcmp(argv[2] , "all" ) == 0 ){
			if( argc > 4 ) SPLIT = atoi(argv[4]);
			else SPLIT = 1;
			if( argc > 5 ) KK = atoi(argv[5]);
                        else KK = 60;
			splitall(&rh,w,filename[j],SPLIT,sw,sw_len,TESTMAX,KK);
			score += rh.hitinf;totalnum += rh.total;recall += rh.hit;scorelT += rh.hitinflT;
			//printf("%d %d %d %d\n",score,totalnum,recall,scorelT);

		}else{
			SPLIT = atoi(argv[2]);
			if( argc > 4 ) KK = atoi(argv[4]);
                        else KK = 60;
			splitsearch(&rh,w,filename[j],SPLIT,sw,sw_len,TESTMAX,KK);
			score += rh.hitinf;totalnum += rh.total;recall += rh.hit;scorelT += rh.hitinflT;
			//printf("%d %d %d %d\n",score,totalnum,recall,scorelT);
		}
	
	}
	printf("%d %d %d %d\n",score,totalnum,recall,scorelT);
	return 0;
}
