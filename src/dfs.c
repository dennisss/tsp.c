#include "tsp.h"

static int depth;
static int *trace;

void tsp_search_dfs(tsp_graph *graph, tsp_path *best){

	tsp_path path;
	tspp_init(&path, graph);

	// Push the first city
	tspp_push(&path, 0);


	// TODO:


}
