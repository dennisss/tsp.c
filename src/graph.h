#ifndef TSP_GRAPH_H_
#define TSP_GRAPH_H_

#define TSP_GRAPH_DENSE 1
#define TSP_GRAPH_SPARSE 2

// Pick which graph implementation to use
#define TSP_GRAPH_IMPL TSP_GRAPH_DENSE



#define NO_EDGE FLT_MAX

// Undirected graphs



#include<float.h>

#include "list.h"




#if TSP_GRAPH_IMPL == TSP_GRAPH_DENSE

// Dense graph implication
// Stores only half of the square as the other half is redundant (for an undirected graph)
typedef struct {
	int size;
	float *edges; // Big buffer of edge weights
	float **levels; // Pointers to the list of edges for each node
} tsp_graph;



#elif TSP_GRAPH_IMPL == TSP_GRAPH_SPARSE



// Sparse graph implementation
typedef struct {
	int size;
	list_t *lists; // List of edges connected to each node
} tsp_graph;


#endif



// Initialize an empty graph with space for the given number of nodes
void tspg_init(tsp_graph *graph, int size);

void tspg_destroy(tsp_graph *graph);

float *tspg_edge(tsp_graph *graph, int i, int j);







#endif

