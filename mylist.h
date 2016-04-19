#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct node{
        int num,count;
	double score;
        struct node *prev,*next;
};

struct list{
        struct node* n;
        int num;
};

struct hitnode{
	int total;
	int hit;
	int hitinf;
	int hitinflT;
};

int newlist(struct list*,int);
int list_add(struct list*,int,int);
int list_add2(struct list*,int,double);
int freelist(struct list*);
void listsort(struct list*);
void printlist(struct list*);
void listscore(struct list*,int,double);
