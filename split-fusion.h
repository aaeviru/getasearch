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
#include <omp.h>
#include "mylist.h"
int splitsearch(struct hitnode* rh,WAM* w,char* filename,int SPLIT,char** sw,int sw_len,int TESTMAX,int KK);
int splitall(struct hitnode* rh,WAM* w,char* filename,int TERMLEN,char** sw,int sw_len,int TESTMAX,int KK);
