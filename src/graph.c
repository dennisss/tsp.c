
#include "graph.h"

#include <stdlib.h>


#if TSP_GRAPH_IMPL == TSP_GRAPH_DENSE



void tspg_init(tsp_graph *graph, int size){

	graph->size = size;

	// Allocate half of the full matrix
	int space = (size * (size + 1)) / 2;
	graph->edges = (float *) malloc(sizeof(float) * space);

	// Initialize the edges
	for(int i = 0; i < space; i++){
		graph->edges[i] = NO_EDGE;
	}


	// Cache level pointers
	graph->levels = (float **) malloc(sizeof(float *) * size);

	float *off = graph->edges;
	for(int i = 0; i < size; i++){
		graph->levels[i] = off;
		off += i + 1;
	}

}

void tspg_destroy(tsp_graph *graph){
	free(graph->levels);
	free(graph->edges);
};

float *tspg_edge(tsp_graph *graph, int i, int j){
	if(i < j)
		return &graph->levels[j][i];
	else
		return &graph->levels[i][j];
};



#elif TSP_GRAPH_IMPL == TSP_GRAPH_SPARSE
// Uses a linked list of edges, where each item has an edge weight and the index of the other node


typedef struct {
	int target;
	float weight;
} tspg_node;






void tspg_init(tsp_graph *graph, int size){
	graph->size = size;

	graph->lists = (list_t *) malloc(sizeof(list_t) * size);

	for(int i = 0; i < size; i++){
		list_init(&graph->lists[i]);
	}
}







#endif
