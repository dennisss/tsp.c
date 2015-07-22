#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tsp.h"
#include "aco.h"
#include "viewer.h"


int main(int argc, char *argv[]){
	srand(time(NULL));


	if(argc != 3){
		printf("Usage: ./tsp data.tsp seconds\n");
		return 1;
	}

	char *infile = argv[1];
	int timelimit = atoi(argv[2]);



	// Read problem
	tsp_problem prob;
	tsp_read(&prob, infile);


	// TODO: Remove cities with the same location



	tsp_viewer viewer;
	tspv_init(&viewer, &prob);



	// Generate graph
	tsp_graph graph;
	tsp_generate_complete(&graph, &prob);
	//tsp_generate_delaunay(&graph, &prob);


	// Initialize a path with a max value
	tsp_path path;
	tspp_init(&path, &graph); path.weight = NO_EDGE;


	// Optimize
	tsp_aco_state state;
	tsp_aco_init(&state, &graph);

	printf("Starting...\n");

	for(int i = 0; i < 10; i++){
		tsp_aco_iterate(&state, &path);
		tspv_update(&viewer, &path);
		printf("#%d: %f\n", i+1, path.weight);
	}

	tsp_aco_destroy(&state);


	printf("Done!\n");


	tspv_wait(&viewer);
	tspv_destroy(&viewer);



	// Print path to file
	FILE *fout = fopen("output.tour", "w+");
	for(int i = 0; i < path.length - 1 /* print all but the last redundant node */; i++){
		int index = path.indices[i];
		fprintf(fout, "%d\n", prob.nodes[index].id);
	}
	fflush(fout);
	fclose(fout);

	// Print to stdout
	printf("%s %f\n", infile, path.weight);





	// Free all the data structures
}
