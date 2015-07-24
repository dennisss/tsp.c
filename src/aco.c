#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "aco.h"

#define NITER 1000
#define NANTS 100

// Ant system parameters
#define ALPHA 1
#define BETA 2
#define RHO 0.5


// Ant System

// Each ant can be represented as a path and a visited table

//




void tsp_aco_init(tsp_aco_state *state, tsp_graph *g, tsp_path *best){
	state->graph = g;

	state->trail = (tsp_graph *) malloc(sizeof(tsp_graph));
	tspg_init(state->trail, g->size);


	// Generate random walk from pheromene initialization
	//tsp_path p; tspp_init(&p, g);
	//p.weight = NO_EDGE;
	tsp_search_rwalk(g, best);

	LOG("Seed Pheromene: %f\n", best->weight);

//	state->probs = (tsp_graph *) malloc(sizeof(tsp_graph));
//	tspg_init(state->probs, g->size);

	// Initialize all pheromones to an initialize guess
	// TODO: Have this copy the edges of the original graph
	for(int i = 0; i < g->size; ++i){
		for(int j = i; j < g->size; ++j){
			*tspg_edge(state->trail, i, j) = 1.0 / best->weight;
		}
	}

	state->ants = (tsp_path *) malloc(sizeof(tsp_path) * NANTS);

	for(int i = 0; i < NANTS; ++i){
		tspp_init(&state->ants[i], state->graph);
	}
}


void tsp_aco_destroy(tsp_aco_state *state){
	for(int i = 0; i < NANTS; ++i){
		tspp_destroy(&state->ants[i]);
	}
	free(state->ants);

	tspg_destroy(state->trail);
	free(state->trail);
}

void tsp_aco_iterate(tsp_aco_state *state, tsp_path *best){

	int n = state->graph->size;

	// Clear and Reset each ant to a random starting city
	for(int i = 0; i < NANTS; ++i){
		tsp_path *a = &state->ants[i];
		tspp_clear(a);
		tspp_push(a, rand() % n);
	}



	// Explore each ant
	for(int i = 0; i < NANTS; ++i){
		tsp_path *a = &state->ants[i];


		// Try to generate solutions
		for(int j = 0; j < n; ++j){
			if(!tsp_aco_step(state, a))
				break;
		}
	}


	// Analyze resultant paths
	for(int i = 0; i < NANTS; ++i){
		tsp_path *a = &state->ants[i];

		// Skip incomplete tours
		if(a->length != (n + 1)) continue;


		// Update the best path
		if(a->weight < best->weight)
			tspp_copy(a, best);


		// Pheromone update
		tsp_aco_update(state, a);
	}


	// Decay
	tsp_aco_decay(state);
}


#include <stdio.h>

inline int tsp_aco_step(tsp_aco_state *state, tsp_path *ant){

	// Generate probabilities for each out edge

	int n = state->graph->size;
	int i = ant->indices[ant->length - 1], j;

	float probs[n];
	//float *probs = (float *) malloc(sizeof(float) * n);

	float sum = 0.0;
	int found = 0;
	for(j = 0; j < n; ++j){

		// TODO: Perform the visited check first

		if(!ant->visited[j]){

			float w = *tspg_edge(state->graph, i, j);

			if(w != NO_EDGE){

				float part = pow(*tspg_edge(state->trail, i, j), ALPHA) / pow(w, 2); //pow(w, BETA);
				sum += part;

				probs[j] = sum;

				found = 1;

				continue;
			}
		}

		probs[j] = 0.0;
	}

	if(!found){ // No edge can be pushed

		// Try to revisit the first city
		tspp_push(ant, ant->indices[0]);


		// Otherwise


		//free(probs);
		return 0;
	}




	// Pick a random edge
	float r = sum * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	//float rolling = 0.0;
	//LOG("%f %f\n", r, sum);

	for(j = 0; j < n; ++j){
		if(r <= probs[j]){ // TODO: Make sure that this skips nodes with no edges

			tspp_push(ant, j);
			break;
		}
	}

	//free(probs);
	return 1;
}


#define Q 1.0
inline void tsp_aco_update(tsp_aco_state *state, tsp_path *ant){
	float dt = Q / ant->weight;

	for(int i = 0; i < ant->length - 1; ++i)
		*tspg_edge(state->trail, ant->indices[i], ant->indices[i+1]) += dt;
}


inline void tsp_aco_decay(tsp_aco_state *state){
	int n = state->graph->size;

	// TODO: Have this copy the edges of the original graph
	for(int i = 0; i < n; ++i){
		for(int j = i; j < n; ++j){
			*tspg_edge(state->trail, i, j) *= (1.0 - RHO);
		}
	}

}





//void tsp_as_iterate(0
