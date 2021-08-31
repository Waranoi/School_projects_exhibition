#include "Window.h"
#include "RayIntersection.h"
#include <stdio.h>
#include <math.h>

GLFWwindow *window;
RayIntersection rayIntersection;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	rayIntersection.KeyCallback(window, key, scancode, action, mods);
}

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	rayIntersection.CursorPositionCallback(window, xpos, ypos);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	rayIntersection.MouseButtonCallback(window, button, action, mods);
}

bool Setup()
{
	//Initialize GLFW and create a window
	window = Window::Create_window(640, 640, "Triangle_app");
	if (window == nullptr)
	{
		printf("Window: Failed to create window\nEnter any button to continue ");
		scanf("%*s");
		return false;
	}

	//Register inputs
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	
	//Set clear color to gray
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

	rayIntersection.Setup(window);
	return true;
}

void Application_loop()
{
	//Program loop
	while (!glfwWindowShouldClose(window))
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		rayIntersection.Run();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main()
{
	if (Setup() == true)
		Application_loop();
	Window::Destroy_window(window);
	return 0;
}