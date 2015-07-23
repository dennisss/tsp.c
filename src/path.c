#include "tsp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tspp_init(tsp_path *p, tsp_graph *g){
	p->graph = g;
	p->indices = (int *) malloc(sizeof(int) * (g->size+1));
	p->visited = (char *) malloc(sizeof(char) * g->size);
	tspp_clear(p);
}

void tspp_destroy(tsp_path *p){
	free(p->indices);
	free(p->visited);
}


void tspp_clear(tsp_path *p){
	p->weight = 0.0;
	p->length = 0;
	memset(p->visited, 0, p->graph->size * sizeof(char));
}

void tspp_push(tsp_path *p, int index){

	if(p->length == p->graph->size + 1){
		LOG("Warning: Path too long\n");
	}

	p->indices[p->length++] = index;
	p->visited[index]++;

	if(p->length > 1){
		p->weight += *tspg_edge(p->graph, p->indices[p->length - 2], p->indices[p->length - 1]);
	}
}

void tspp_pop(tsp_path *p){
	int lasti = p->indices[p->length - 1];

	if(p->length > 1){
		p->weight -= *tspg_edge(p->graph, p->indices[p->length - 2], p->indices[p->length - 1]);
	}

	p->visited[lasti]--;
	p->length--;
}

void tspp_copy(tsp_path *p, tsp_path *dst){
	dst->weight = p->weight;
	dst->length = p->length;
	for(int i = 0; i < p->length; i++)
		dst->indices[i] = p->indices[i];
}
