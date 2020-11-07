#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <assert.h>

namespace Window
{
	inline void Error_callback(int error, const char *description)
	{
		printf("Error: %s\n", description);
	}
	
	inline GLFWwindow* Create_window(int width, int height, char const *title)
	{
		//Initialize GLFW
		if (!glfwInit())
		{
			//Initialization failed.
			printf("GLFW: Failed to initialize\n");
			return nullptr;
		}
		
		//Set error callback
		glfwSetErrorCallback(Error_callback);
		
		//Create window
		GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window)
		{
			//Window creation failed.
			printf("GLFW: Failed to create window\n");
			glfwTerminate();
			return nullptr;
		}
		
		//Initialize Context
		glfwMakeContextCurrent(window);
		gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	
		//vsync
		glfwSwapInterval(1);
		
		return window;
	}
	
	inline void Destroy_window(GLFWwindow *window)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

#endif //WINDOW_H