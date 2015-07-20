#include <stdio.h>
#include <stdlib.h>

#include "tsp.h"
#include "aco.h"


int main(int argc, char *argv[]){

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


	// Generate graph
	tsp_graph graph;

	tsp_generate_complete(&graph, &prob);
	//tsp_generate_delaunay(&graph, &prob);



	// Optimize
	tsp_path path;







	// Print path to file
	FILE *fout = fopen("output.tour", "w+");
	for(int i = 0; i < path.size - 1 /* print all but the last redundant node */; i++){
		int index = path.indices[i];
		fprintf(fout, "%d\n", prob.nodes[index].id);
	}
	fflush(fout);
	fclose(fout);

	// Print to stdout
	printf("%s %f\n", infile, path->weight);





	// Free all the data structures
}
