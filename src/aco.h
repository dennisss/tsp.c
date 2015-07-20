#ifndef TSP_ACO_H_
#define TSP_ACO_H_

#include "tsp.h"



// Initialize a ACO search
void tsp_aco_init(tsp_aco_state *state, tsp_graph *g);
void tsp_aco_destroy(tsp_aco_state *state);

// Iterate the search process and try to improve the best known path
void tsp_aco_iterate(tsp_aco_state *state, tsp_path *best);





typedef struct {
	tsp_graph *graph;
	tsp_graph *trail;

	tsp_ant *ants;

} tsp_aco_state;



typedef struct {
	tsp_path path;
	char *visited;
} tsp_ant;


void tsp_ant_init(tsp_ant *a, tsp_graph *g);
void tsp_ant_destroy(tsp_ant *a);

// Clear an ant back to its initial state
void tsp_ant_clear(tsp_ant *a);


// Have the ant move up one node
void tsp_ant_step(tsp_ant *a, tsp_aco_state *state);




#endif
