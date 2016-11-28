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

WAM* init(char* wam);
void fin(WAM* w);
int search(WAM* w,int argc,char* argv[],int* r,int maxnum);
