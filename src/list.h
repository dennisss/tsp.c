#ifndef LIST_H_
#define LIST_H_


typedef struct list_t list_t;
struct list_t {
	list_t *next;
	char data[];
};



void list_init();




#endif
