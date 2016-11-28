%module getapy
%{
#include "getapy.h"
#define NaN (0.0/0.0)
%}
// Now list ANSI C/C++ declarations
%include <argcargv.i>
%include "carrays.i"
%include "cdata.i"
%apply (int ARGC, char **ARGV) { (int argc, char *argv[]) }
%array_class(int,intp);
extern void fin(WAM* w);
extern WAM* init(char* wam);
extern int search(WAM* w,int argc,char* argv[],int* r,int maxnum);
