#ifndef TSP_VIEWER_H_
#define TSP_VIEWER_H_

#include "tsp.h"

#define VIEWER_SHOW_EDGES 1

#include <pthread.h>

typedef struct {
	tsp_problem *prob;


	pthread_t thread;
	pthread_mutex_t lock;


	int dirty; // Whether or not the data has been updated
	int flags;

	void *cityVertices; int nCityVerts; int citySize;

	void *pathIndices; int nPathInds; int pathSize;
	void *edgeIndices; int nEdgeInds; int edgeSize;

} tsp_viewer;


// Opens an empty window for displaying graphs and paths
// Updates are performs in a separate UI thread
void tspv_init(tsp_viewer *viewer, tsp_problem *prob, tsp_graph *graph, int flags);

// Cleanup the viewer
void tspv_destroy(tsp_viewer *viewer);

// Update the path currently being displayed
void tspv_update(tsp_viewer *viewer, tsp_path *path);

// Wait for the viewer to close
void tspv_wait(tsp_viewer *viewer);



#endif
