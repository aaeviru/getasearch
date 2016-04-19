#include "mylist.h"
int newlist(struct list* l,int len){
	l->n = NULL;
	if( NULL==(l->n = malloc(len * sizeof(struct node))) ){
		return -1;
	}
	l->num = 0;
	return 0;
}

int list_add(struct list* l,int num,int K){
	struct node* p = &(l->n[l->num]);
	p->num = num;
	p->score = 1.0 / (K + l->num);
	l->num++;
	return 0;
}
	
int list_add2(struct list* l,int num,double score){
	struct node* p = &(l->n[l->num]);
	p->num = num;
	p->score = score;
	l->num++;
        return 0;
}

int freelist(struct list* l){
	if(l->n != NULL) free(l->n);
	l->n = NULL;
	return 0;
}
/*
void swapint(struct node* head, int i, int j){
	int k = i<j?i:j;
	struct node* tmp = head;
    	int tmpnum;
	double tmpscore;
    	while(head && k) {
        	k--;
        	head = head->next;
    	}
	tmp = head;
	k = abs(i-j);
    	while(head && k) {
        	k--;
        	head = head->next;
    	}
    	tmpnum = tmp->num;
	tmpscore = tmp->score;
    	tmp->num = head->num;
	tmp->score = head->score;
    	head->num = tmpnum;
	head->score = tmpscore;
}

void swap(struct node* p,struct node* q){
	int tmpnum = p->num;
	double tmpscore = p->score;
	p->num = q->num;
	p->score = q->score;
	q->num = tmpnum;
	q->score = tmpscore;
	return;
}

struct node* get_node(struct node* head, int l){
	while(head && l) {
        	head = head->next;
        	l--;
    	}
	return head;
}
	
void Quick_Sort_List(struct node *head, int r){

	int i = 1, j;
    	double jval;
    	double pivot;
	struct node *p = head->next,*q = p;
    	if ( 1 < r) {
        	pivot = head->score;
        	for (j = 1; j <= r; j++) {
            		if ( q->next && q->score > pivot ) {
                		swap(p,q);
                		p = p->next;
				i++;
            		}
			q = q->next;
        	}
		printf("%d %d\n",i,r);
        	swap(p->prev,head);
		printf("%lf %lf\n",p->prev->score,head->score);
        	Quick_Sort_List(head,i-1);
        	Quick_Sort_List(p,r-i);
    	}
    	return;
}
*/
int cmpfunc (const void * a, const void * b){
   	if (( ((struct node*)b)->score - ((struct node*)a)->score ) < 0) return -1;
	return 1;
}
void listsort(struct list* l){
	qsort(l->n,l->num,sizeof(struct node),cmpfunc);
	return;
}

void printlist(struct list* l){
	struct node* p = l->n;
	int i;
	for(i=0;i<l->num;i++){
		printf("%d %lf\n",p[i].num,p[i].score);
	}
	return;
}

void listscore(struct list *l,int count,double c){
	struct node* p = l->n;
	int i;
	for(i=0;i<l->num;i++){
		p[i].score += (count - p[i].count) * c;
	}
	return;
}
