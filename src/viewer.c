/*

#include <GLFW/glfw3.h>
#include <thread>

#define GLEW_STATIC
#include <GL/glew.h>


void tspv_show(){
    glfwInit();
    std::this_thread::sleep_for(std::chrono::seconds(1));

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "TSP View", nullptr, nullptr);

	glfwMakeContextCurrent(window);


	glewExperimental = GL_TRUE;
	glewInit();


	while(!glfwWindowShouldClose(window)){




		glfwSwapBuffers(window);
		glfwPollEvents();
	}





    glfwTerminate();
}

*/
