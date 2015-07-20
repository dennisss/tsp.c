#ifndef ARRAY_H_
#define ARRAY_H_



typedef struct {
	int size;
	int capacity;
	void **data;
} array_t;


void array_init(array_t *a);

void array_add(array_t *a, void *val);

void array_clear(array_t *a);

void **array_element(array_t *a, int i);

void array_destroy(array_t *a);



#endif
