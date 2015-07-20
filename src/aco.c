
#include <stdlib.h>

#include "aco.h"

#define NITER 1000
#define NANTS 100

// Ant System

// Each ant can be represented as a path and a visited table

//




void tsp_aco_init(tsp_aco_state *state, tsp_graph *g){
	state->graph = g;

	state->trail = (tsp_graph *) malloc(sizeof(tsp_graph));
	tspg_init(state->trail, g->size);

	state->ants = (tsp_ant *) malloc(sizeof(tsp_ant) * NANTS);

	for(int i = 0; i < NANTS; i++){
		tsp_ant_init(&state->ants[i], state->graph);
	}
}


void tsp_aco_destroy(tsp_aco_state *state){



}

void tsp_aco_iterate(tsp_aco_state *state, tsp_path *best){

	int n = state->graph->size;

	// Clear and Reset each ant to a random starting city



	// Explore randomly for each ant
	for(int i = 0; i < NANTS; i++){
		tsp_ant *a = &state->ants[i];

		// Generate complete tours
		// TODO: The tours may fail, so be sure to check the length of the generated path
		for(int i = 0; i < n; i++)
			tsp_ant_step(a, state);
	}

	// Update the best path


	// Pheromone update




	free(ants);

}






void tsp_ant_clear(tsp_ant *a){


}

/////// OLD //////


void tsp_as_init(tsp_graph *graph, tsp_path *bestpath){

	tspp_init(graph->size);



	char *visited = (char *) malloc(sizeof(char) * graph->size);

	tsp_graph trail;




	for(int iter = 0; iter < NITER; iter++){




	}


	free(visited);
}


//void tsp_as_iterate(0
