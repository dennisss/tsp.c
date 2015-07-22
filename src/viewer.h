#ifndef TSP_VIEWER_H_
#define TSP_VIEWER_H_

#include "tsp.h"

#include <pthread.h>

typedef struct {
	tsp_problem *prob;


	pthread_t thread;
	pthread_mutex_t lock;


	int dirty; // Whether or not the data has been updated

	float *cityVertices; int nCityVerts; int citySize;
	float *pathVertices; int nPathVerts; int pathSize;
} tsp_viewer;


// Opens an empty window for displaying graphs and paths
// Updates are performs in a separate UI thread
void tspv_init(tsp_viewer *viewer, tsp_problem *prob);

// Cleanup the viewer
void tspv_destroy(tsp_viewer *viewer);

// Update the path currently being displayed
void tspv_update(tsp_viewer *viewer, tsp_path *path);

// Wait for the viewer to close
void tspv_wait(tsp_viewer *viewer);



#endif
