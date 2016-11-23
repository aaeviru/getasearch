%module tstwam 
%{
#include "tstwam.h"
#define NaN (0.0/0.0)
%}
// Now list ANSI C/C++ declarations
%include <argcargv.i>
%apply (int ARGC, char **ARGV) { (int argc, char *argv[]) }
int search(int argc,char* argv[]);
