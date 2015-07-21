#ifndef TSP_H_
#define TSP_H_


#include "graph.h"

// Represents a city in the TSP problem
typedef struct { // Node: node ids start at 1, not 0
	int id;

	float x;
	float y;
} tsp_node;

// Description of the TSP problem
typedef struct {
	char *name;

	int size;
	tsp_node *nodes;
} tsp_problem;


typedef struct {
	tsp_graph *graph;

	float weight; // Total weight of the path
	int length; // Number of nodes traveled (including the initial one)
	int *indices; // Indices of the nodes visited in order of travel (for a tour, the first and last index will be the same)

	char *visited; // List of boolean characters indicating which nodes where visited
} tsp_path;

// Initialize a new zeroed path
void tspp_init(tsp_path *p, tsp_graph *g);
void tspp_destroy(tsp_path *p);
void tspp_clear(tsp_path *p);
void tspp_push(tsp_path *p, int index);
void tspp_pop(tsp_path *p);
void tspp_copy(tsp_path *p, tsp_path *dst);

// Read a file into an existing problem object
void tsp_read(tsp_problem *prob, char *filename);



/*
typedef void (*tsp_graph_generator)(tsp_graph*, tsp_problem*);
typedef void (*tsp_graph_searcher)(tsp_graph*, tsp_path*);

void tsp_solve(tsp_graph_generator gg, tsp_graph_searcher gs);
*/


// Generators
void tsp_generate_complete(tsp_graph *g, tsp_problem *p);
void tsp_generate_delaunay(tsp_graph *g, tsp_problem *p);


// Searchers
void tsp_search_dfs(tsp_graph *graph, tsp_path *path);


#endif


