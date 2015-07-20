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

		tokenize(line, &toks); //spacetok(line.c_str());
		// TODO: Could it be null here

		if(toks.size == 0)
			continue;


		if(inheader){
			if(strcmp(toks.data[0], "NAME") == 0){


			}
			else if(strcmp(toks.data[0], "DIMENSION") == 0){ // DIMENSION : [size]
				prob->size = atoi(toks.data[2]);
				prob->nodes = (tsp_node *) malloc(sizeof(tsp_node) * prob->size);
			}
			else if(strcmp(toks.data[0], "NODE_COORD_SECTION") == 0){
				inheader = 0;
			}
		}
		else{ // In node body
			if(strcmp(toks.data[0], "EOF") == 0){ // TODO: Make sure that the \n doesn't mess it up
				break;
			}


			int id = atoi(toks.data[0]);
			tsp_node *node = &prob->nodes[n++];

			node->id = id;
			node->x = atof(toks.data[1]);
			node->y = atof(toks.data[2]);
		}
	}


	array_destroy(&toks);


	free(line);
	fclose(f);
}





