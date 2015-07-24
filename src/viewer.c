#ifdef BUILD_VIEWER

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "viewer.h"
#include "tsp.h"
#include "shader.h"

#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>


/*

	TODO: Why is everything displayed sideways?
*/

#define PERCENT_PAD 0.05

void *_tspv_uithread(void *arg);

void tspv_init(tsp_viewer *viewer, tsp_problem *prob, tsp_graph *graph, int flags){
	if(prob->size == 0){
		printf("Problem must have at least one node to visualize\n");
		return;
	}

	viewer->prob = prob;
	viewer->lock = PTHREAD_MUTEX_INITIALIZER;
	viewer->dirty = 0;
	viewer->flags = flags;

	// Allocate buffers


	// Load all the city nodes
	viewer->nCityVerts = prob->size;
	viewer->citySize = 2 * sizeof(GLfloat) * viewer->nCityVerts;

	GLfloat *cityVertices = (GLfloat *) malloc(viewer->citySize);
	viewer->cityVertices = cityVertices;
	for(int i = 0; i < viewer->nCityVerts; i++){
		cityVertices[2*i] = prob->nodes[i].x;
		cityVertices[2*i+1] = prob->nodes[i].y;
	}



	if(viewer->flags & VIEWER_SHOW_EDGES){

		// Load the edges
		int nEdges = 0;
		for(int i = 0; i < graph->size; i++){
			for(int j = i+1; j < graph->size; j++){
				if(*tspg_edge(graph, i, j) != NO_EDGE)
					nEdges++;
			}
		}

		viewer->nEdgeInds = 2 * nEdges;
		viewer->edgeSize = viewer->nEdgeInds * sizeof(GLuint);

		GLuint *edgeIndices = (GLuint *) malloc(viewer->edgeSize);
		viewer->edgeIndices = edgeIndices;

		nEdges = 0;
		for(int i = 0; i < graph->size; i++){
			for(int j = i+1; j < graph->size; j++){
				if(*tspg_edge(graph, i, j) != NO_EDGE){
					GLuint *e = &edgeIndices[2*nEdges];

					e[0] = i;
					e[1] = j;

					nEdges++;
				}
			}
		}

	}


	// Initialize zeroed path
	viewer->nPathInds = prob->size + 1;
	viewer->pathSize = sizeof(GLuint) * viewer->nPathInds;

	GLuint *pathIndices = (GLuint *) malloc(viewer->pathSize);
	viewer->pathIndices = pathIndices;
	for(int i = 0; i < viewer->nPathInds; i++){
		pathIndices[i] = 0;
	}



	pthread_create(&viewer->thread, NULL, _tspv_uithread, viewer);

}

void tspv_destroy(tsp_viewer *viewer){

	free(viewer->cityVertices);
	free(viewer->pathIndices);

	if(viewer->flags & VIEWER_SHOW_EDGES)
		free(viewer->edgeIndices);

	// Destroy the vertex buffers and the shader programs


}

void tspv_update(tsp_viewer *viewer, tsp_path *path){
	pthread_mutex_lock(&viewer->lock);

	GLuint *pathIndices = (GLuint *) viewer->pathIndices;

	for(int i = 0; i < path->length; i++){
		pathIndices[i] = path->indices[i];
	}

	viewer->dirty = 1;
	pthread_mutex_unlock(&viewer->lock);
}

void tspv_wait(tsp_viewer *viewer){
	pthread_join(viewer->thread, NULL);
}






void *_tspv_uithread(void *arg){
	tsp_viewer *viewer = (tsp_viewer *) arg;


	// Get a bounding box
	float xmin = FLT_MAX, xmax = FLT_MIN, ymin = FLT_MAX, ymax = FLT_MIN;
	for(int i = 0; i < viewer->prob->size; i++){
		tsp_node *n = &viewer->prob->nodes[i];
		if(n->x < xmin) xmin = n->x;
		if(n->x > xmax) xmax = n->x;
		if(n->y < ymin) ymin = n->y;
		if(n->y > ymax) ymax = n->y;
	}
	float xspan = xmax - xmin, yspan = ymax - ymin;

	// Add padding
	float xpad = PERCENT_PAD*xspan, ypad = PERCENT_PAD * yspan;
	xmin -= xpad; xmax += xpad;
	ymin -= ypad; ymax += ypad;
	xspan += 2*xpad;
	yspan += 2*ypad;


	int wwidth, wheight; // Window dimensions
	if(xspan > yspan){
		wwidth = 800;
		wheight = (yspan / xspan) * 800;
	}
	else{
		wwidth = (xspan / yspan) * 800;
		wheight = 800;
	}
	// float aspect = ...





	/// Initialize the window
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(wwidth, wheight, "TSP View", NULL, NULL);

	glfwMakeContextCurrent(window);


	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		printf("GLEW Failed!\n");


	GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);



	// Shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
		printf("Vertex shader failed to compile\n");

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
		printf("Fragment shader failed to compile\n");

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);





	// Setup view matrix
	GLfloat view[] = {
		2.0/xspan, 0, 0, -2.0*xmin/xspan - 1.0,
		0, 2.0/yspan, 0, -2.0*ymin/yspan - 1.0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	GLint uniView = glGetUniformLocation(shaderProgram, "viewport");
	glUniformMatrix4fv(uniView, 1, GL_TRUE, view);


	GLint uniColor = glGetUniformLocation(shaderProgram, "Color");
	glUniform3f(uniColor, 0.0, 0.0, 0.0);






	// Create city point array
	GLuint vboCities;
	glGenBuffers(1, &vboCities);
	glBindBuffer(GL_ARRAY_BUFFER, vboCities);
	glBufferData(GL_ARRAY_BUFFER, viewer->citySize, viewer->cityVertices, GL_STATIC_DRAW);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);




	// Create edge array
	GLuint vboEdges;
	if(viewer->flags & VIEWER_SHOW_EDGES){
		glGenBuffers(1, &vboEdges);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboEdges);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, viewer->edgeSize, viewer->edgeIndices, GL_STATIC_DRAW);
	}


	// Generate path vertices
	GLuint vboPath;
	glGenBuffers(1, &vboPath);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboPath);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, viewer->pathSize, viewer->pathIndices, GL_STATIC_DRAW);






	glEnable(GL_POINT_SMOOTH);
	glPointSize(4.0);


	// UI Loop
	while(!glfwWindowShouldClose(window)){

		// Update the path vertices
		if(viewer->dirty){
			pthread_mutex_lock(&viewer->lock);

			// Upload new vertices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboPath);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, viewer->pathSize, viewer->pathIndices, GL_STATIC_DRAW);

			viewer->dirty = 0;
			pthread_mutex_unlock(&viewer->lock);
		}



		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw edges
		if(viewer->flags & VIEWER_SHOW_EDGES){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboEdges);
			glUniform3f(uniColor, 0.8, 0.8, 0.8);
			glDrawElements(GL_LINES, viewer->nEdgeInds, GL_UNSIGNED_INT, NULL);
		}

		// Draw path
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboPath);
		glUniform3f(uniColor, 0.0, 0.0, 0.0);
		glDrawElements(GL_LINE_STRIP, viewer->nPathInds, GL_UNSIGNED_INT, NULL);


		// Draw cities
		glUniform3f(uniColor, 1.0, 0.0, 0.0);
		//glBindBuffer(GL_ARRAY_BUFFER, vboCities);
		glDrawArrays(GL_POINTS, 0, viewer->nCityVerts);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}


    glfwTerminate();


	return NULL;
}



#endif
