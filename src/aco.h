#ifndef TSP_ACO_H_
#define TSP_ACO_H_

#include "tsp.h"


typedef struct {
	tsp_graph *graph;
	tsp_graph *trail;

	tsp_path *ants;

} tsp_aco_state;


// Initialize a ACO search
void tsp_aco_init(tsp_aco_state *state, tsp_graph *g, tsp_path *best);
void tsp_aco_destroy(tsp_aco_state *state);

// Iterate the search process and try to improve the best known path
void tsp_aco_iterate(tsp_aco_state *state, tsp_path *best);


// Have the ant move up one node, returns true if successful
int tsp_aco_step(tsp_aco_state *state, tsp_path *ant);


void tsp_aco_update(tsp_aco_state *state, tsp_path *ant);
void tsp_aco_decay(tsp_aco_state *state);

#endif
