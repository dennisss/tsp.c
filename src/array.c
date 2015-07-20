// Dynamic array implementation

#include <stdlib.h>

#include "array.h"

#define INITIAL_SIZE 2

void array_init(array_t *a){
	a->size = 0;
	a->capacity = INITIAL_SIZE;
	a->data = (void **) malloc(sizeof(void *) * INITIAL_SIZE);
}


void array_add(array_t *a, void *val){
	// Expand
	if(a->size == a->capacity){
		a->capacity = a->capacity * 2;
		a->data = (void **)realloc(a->data, sizeof(void *) * a->capacity);
	}

	*array_element(a, a->size++) = val;
}

void array_clear(array_t *a){
	a->size = 0;
}

void **array_element(array_t *a, int i){
	return &a->data[i];
};

void array_destroy(array_t *a){
	free(a->data);
};
