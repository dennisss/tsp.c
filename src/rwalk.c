#include "tsp.h"

#include <stdio.h>
#include <stdlib.h>

// Random walk biased on edge weight


int graph_random_pick(tsp_graph *graph, tsp_path *p, int i);

void tsp_search_rwalk(tsp_graph *graph, tsp_path *best){
	tsp_path path;
	tspp_init(&path, graph);

	int n = graph->size;

	// Starting city
	tspp_push(&path, rand() % n);

	for(int i = 0; i < n - 1; ++i){

		int last = path.indices[i];
		int next = graph_random_pick(graph, &path, last);

		if(next < 0) // No possible edges
			break;

		tspp_push(&path, next);
	}

	// Revisit first city
	tspp_push(&path, path.indices[0]);


	if(path.length != graph->size + 1){
		LOG("Random Walk Failed!\n");
		return;
	}

	if(path.weight < best->weight)
		tspp_copy(&path, best);

}


// Returns the next index to go to, or -1 if non possible
int graph_random_pick(tsp_graph *graph, tsp_path *p, int i){

	int n = graph->size, j;

	float probs[n];
	float sum = 0.0;
	int found = 0;
	for(j = 0; j < n; ++j){
		float w = *tspg_edge(graph, i, j);

		if(w != NO_EDGE && !p->visited[j]){
			float part = 1.0 / w;
			probs[j] = part;
			sum += part;
			found = 1;
		}
		else
			probs[j] = 0.0;
	}

	if(!found){ // No edge can be pushed
		return -1;
	}




	// Pick a random edge
	float r = sum * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float rolling = 0.0;

	for(j = 0; j < n; ++j){
		rolling += probs[j];
		if(r <= rolling){ // TODO: Make sure that this skips nodes with no edges
			return j;
			break;
		}
	}

	return -1;
}
