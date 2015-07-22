#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "viewer.h"
#include "tsp.h"
#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#define PERCENT_PAD 0.05

void *_tspv_uithread(void *arg);

void tspv_init(tsp_viewer *viewer, tsp_problem *prob){ //, tsp_path *path){
	if(prob->size == 0){
		printf("Problem must have at least one node to visualize\n");
		return;
	}

	viewer->prob = prob;
	viewer->lock = PTHREAD_MUTEX_INITIALIZER;
	viewer->dirty = 0;


	// Allocate buffers


	// Load all the city nodes
	viewer->nCityVerts = prob->size;
	viewer->citySize = 2 * sizeof(float) * viewer->nCityVerts;
	viewer->cityVertices = (float *) malloc(viewer->citySize); // TODO: This memory needs to be free'd
	for(int i = 0; i < viewer->nCityVerts; i++){
		viewer->cityVertices[2*i] = prob->nodes[i].x;
		viewer->cityVertices[2*i+1] = prob->nodes[i].y;
	}




	// Initialize zeroed path
	viewer->nPathVerts = prob->size + 1;
	viewer->pathSize = 2 * sizeof(float) * viewer->nPathVerts;
	viewer->pathVertices = (float *) malloc(viewer->pathSize);
	for(int i = 0; i < viewer->nPathVerts; i++){
		viewer->pathVertices[2*i] = 0;
		viewer->pathVertices[2*i+1] = 0;
	}



	pthread_create(&viewer->thread, NULL, _tspv_uithread, viewer);

}

void tspv_destroy(tsp_viewer *viewer){

	free(viewer->cityVertices);
	free(viewer->pathVertices);


	// Destroy the vertex buffers and the shader programs


}

void tspv_update(tsp_viewer *viewer, tsp_path *path){
	pthread_mutex_lock(&viewer->lock);

	printf("%d\n", path->length);

	for(int i = 0; i < path->length; i++){
		int a = path->indices[i];

		viewer->pathVertices[2*i] = viewer->prob->nodes[a].x;
		viewer->pathVertices[2*i+1] = viewer->prob->nodes[a].y;
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

	GLFWwindow *window = glfwCreateWindow(wwidth, wheight, "TSP View", nullptr, nullptr);

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
	float view[] = {
		2.0/xspan, 0, 0, -2.0*xmin/xspan - 1.0,
		0, 2.0/yspan, 0, -2.0*ymin/yspan - 1.0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	GLint uniView = glGetUniformLocation(shaderProgram, "viewport");
	glUniformMatrix4fv(uniView, 1, GL_TRUE, view);



	// Create city point array
	GLuint vboCities;
	glGenBuffers(1, &vboCities);
	glBindBuffer(GL_ARRAY_BUFFER, vboCities);
	glBufferData(GL_ARRAY_BUFFER, viewer->citySize, viewer->cityVertices, GL_STATIC_DRAW);



	// Generate path vertices
	GLuint vboPath;
	glGenBuffers(1, &vboPath);
	glBindBuffer(GL_ARRAY_BUFFER, vboPath);
	glBufferData(GL_ARRAY_BUFFER, viewer->pathSize, viewer->pathVertices, GL_STATIC_DRAW);



	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);






	glEnable(GL_POINT_SMOOTH);
	glPointSize(4.0);


	// UI Loop
	while(!glfwWindowShouldClose(window)){

		// Update the path vertices
		if(viewer->dirty){
			pthread_mutex_lock(&viewer->lock);

			// Upload new vertices
			glBindBuffer(GL_ARRAY_BUFFER, vboPath);
			glBufferData(GL_ARRAY_BUFFER, viewer->pathSize, viewer->pathVertices, GL_STATIC_DRAW);

			viewer->dirty = 0;
			pthread_mutex_unlock(&viewer->lock);
		}



		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw cities
		glBindBuffer(GL_ARRAY_BUFFER, vboCities);
		glDrawArrays(GL_POINTS, 0, viewer->nCityVerts);

		// Draw path
		glBindBuffer(GL_ARRAY_BUFFER, vboPath);
		glDrawArrays(GL_LINE_STRIP, 0, viewer->nPathVerts);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


    glfwTerminate();


	return NULL;
}

