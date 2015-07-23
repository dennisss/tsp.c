#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "tsp.h"
#include "aco.h"
#include "viewer.h"

static tsp_problem prob;
static tsp_path path; // TODO: This needs to be locked

static tsp_aco_state state;

static tsp_viewer viewer;
static tsp_graph graph;

static clock_t starttime;

static int stillrunning;
static pthread_t thread = NULL;


void catch_alarm(int sig){
	if(thread != NULL)
		pthread_cancel(thread);
}
void catch_interrupt(int sig){
	catch_alarm(sig);
}


// Arguments
static int show_viewer = 0;




static void *compute(void *arg){
	// TODO: This method is a little dangerous
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	clock_t t;

	for(int i = 1; ; i++){
		tsp_aco_iterate(&state, &path);

		#ifdef BUILD_VIEWER
		if(show_viewer)
			tspv_update(&viewer, &path);
		#endif

		t = clock() + starttime;

		LOG("%d %f %f\n", i, ((float)t)/CLOCKS_PER_SEC, path.weight);
	}
}



int main(int argc, char *argv[]){
	starttime = clock();
	srand(time(NULL));


	// Setup termination signals
	signal(SIGALRM, catch_alarm);
	signal(SIGINT, catch_interrupt);


	if(argc < 3){
		printf("Usage: ./tsp data.tsp seconds [-view] [-method (aco|aco+del|dfs+del)] [-init best.tour] \n");
		return 1;
	}

	char *infile = argv[1];
	int timelimit = atoi(argv[2]);

	for(int i = 3; i < argc; i++){
		char *arg = argv[i];
		if(strcmp(arg, "-view") == 0){
			show_viewer = 1;
		}
	}


	if(timelimit != 0){
		alarm(timelimit == 1? 1 : timelimit - 1);
	}



	// Read problem
	tsp_read(&prob, infile);

	// Remove cities with the same location
	LOG("Total Cities: %d\n", prob.size);
	tsp_merge(&prob);
	LOG("Unique Cities: %d\n", prob.size);




	#ifdef BUILD_VIEWER
	if(show_viewer)
		tspv_init(&viewer, &prob);
	#endif



	// Generate graph
	tsp_generate_complete(&graph, &prob);
	//tsp_generate_delaunay(&graph, &prob);


	// Initialize a path with a max value
	tspp_init(&path, &graph); path.weight = NO_EDGE;


	// Optimize
	LOG("Starting...\n");
	tsp_aco_init(&state, &graph, &path);


	// Start running
	pthread_create(&thread, NULL, compute, NULL);
	pthread_join(thread, NULL);

	tsp_aco_destroy(&state);
	LOG("Done!\n");


	#ifdef BUILD_VIEWER
	if(show_viewer){
		tspv_wait(&viewer);
		tspv_destroy(&viewer);
	}
	#endif


	// Print path to file
	FILE *fout = fopen("output.tour", "w+");
	for(int i = 0; i < path.length - 1 /* print all but the last redundant node */; i++){
		int index = path.indices[i];

		tsp_node *n = &prob.nodes[index];
		for(; n != NULL; n = n->next) // Print the whole list of same cities
			fprintf(fout, "%s\n", n->id);
	}
	fflush(fout);
	fclose(fout);

	// Print to stdout
	printf("%s %f\n", infile, path.weight);



	// TODO: Free all the data structures
}
