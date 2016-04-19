#include "mylist.h"
#include <stdio.h>

int main(){
	struct list ll;
	struct list* l = &ll;
	newlist(l);
	list_add(l,200);
	printf("%lf\n",l->head->next->score);
	freelist(l);
	return 0;
}
