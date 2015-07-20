#include <math.h>

#include "../tsp.h"
#include "../graph.h"


// Generate a complete graph from the TSP problem with n^2 edges



void tsp_generate_complete(tsp_graph *g, tsp_problem *p){
	int size = p->size, i, j;
	tsp_node *n, *m;

	tspg_init(g, size);

	for(i = 0; i < size; ++i){
		n = &p->nodes[i];
		*tspg_edge(g, i, i) = 0;

		for(j = i+1; j < size; ++j){
			m = &p->nodes[j];

			*tspg_edge(g, i, j) = hypot(n->x - m->x, n->y - m->y);
		}
	}
}
