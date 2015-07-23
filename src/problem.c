// Reader for data files from TSPLIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tsp.h"
#include "utils.h"
#include "array.h"

void tokenize(char *str, array_t *toks){
	array_clear(toks);

	char *t = spacetok(str);
	while(t != NULL){
		array_add(toks, t);
		t = spacetok(NULL);
	}
}


// TODO: Error check the read
void tsp_read(tsp_problem *prob, char *filename){

	FILE *f = fopen(filename, "r");

	if(f == NULL){
		// Fail to open
		return;
	}


	size_t buf_size = 512;
	char *line = (char *)malloc(buf_size);

	array_t toks; array_init(&toks);


	int inheader = 1;

	int n = 0;
	while(getline(&line, &buf_size, f) >= 0){


		// TODO: Skip empty lines

		 //spacetok(line.c_str());
		// TODO: Could it be null here

//		if(toks.size == 0)
//			continue;


		if(inheader){
			char *key, *value;
			keyvalue_split(line, &key, &value);

			if(strcmp(key, "NAME") == 0){


			}
			else if(strcmp(key, "DIMENSION") == 0){
				prob->size = atoi(value);
				prob->nodes = (tsp_node *) malloc(sizeof(tsp_node) * prob->size);
			}
			else if(strcmp(key, "NODE_COORD_SECTION") == 0){
				inheader = 0;
			}
		}
		else{ // In node body

			tokenize(line, &toks);

			if(strcmp(toks.data[0], "EOF") == 0){ // TODO: Make sure that the \n doesn't mess it up
				break;
			}

			tsp_node *node = &prob->nodes[n++];

			node->id = strdup(toks.data[0]);
			node->x = atof(toks.data[1]);
			node->y = atof(toks.data[2]);
			node->next = NULL;
		}
	}


	array_destroy(&toks);


	free(line);
	fclose(f);
}



static int node_compare(const void *va, const void *vb){
	tsp_node *a = (tsp_node *)va, *b = (tsp_node *)vb;

	if(a->x < b->x)
		return -1;
	else if(a->x > b->x)
		return 1;
	else if(a->y < b->y)
		return -1;
	else if(a->y > b->y)
		return 1;

	return 0;
}


// Duplicate merging in O(nlogn + n) time
void tsp_merge(tsp_problem *prob){
	qsort(prob->nodes, prob->size, sizeof(tsp_node), node_compare);

	// TODO: This will fail for a 0 node problem
	int i = prob->size - 1;
	tsp_node *last = &prob->nodes[i]; // The current node being compared


	int orig = prob->size;

	i--;
	for(; i >= 0; --i){
		tsp_node *cur = &prob->nodes[i];

		if(cur->x == last->x && cur->y == last->y){ // Duplicate

			// Swap last with current
			tsp_node *end = &prob->nodes[prob->size - 1];

			if(last == end){
				cur->next = end;
				last = cur;
			}
			else{

				tsp_node temp = *end;
				*end = *cur;
				*cur = temp;

				last->next = end;
				last = end;

			}

			--prob->size;
		}
		else
			last = cur;

	}

}




