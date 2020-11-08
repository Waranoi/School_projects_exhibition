#include "Window.h"
#include "HE_mesh_V3.h"
#include "MeshResource.h"
#include <stdio.h>
#include <math.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <chrono>

GLFWwindow *window;
GLuint program;
GLuint vs_int;
GLuint fs_int;
GLuint triangle;
GLuint triangle_ebo;

//GraphicsNode cube;
MeshResource *myMesh;
HE_mesh_V3 *meshData;

const GLchar* vs =
"#version 430\n"
"uniform mat4 matrix;\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec2 uvIn;\n"
"layout(location=2) in vec3 norm;\n"
"layout(location=0) out vec2 uvOut;\n"
"layout(location=1) out vec3 vertPos;\n"
"layout(location=2) out vec3 normal;\n"
"void main()\n"
"{\n"
"	//Apply projection after translation and rotation\n"
"	vec4 position = matrix * (vec4(pos, 1)) + vec4(0, 0, -2, 0);\n"
"   position[3] = -position[2];\n" 
"	gl_Position = position;\n"
"	\n"
"	uvOut.x = uvIn.x;\n"
"	uvOut.y = -uvIn.y;\n"
"	\n"
"	vertPos = (matrix * vec4(pos, 1)).xyz;\n"
"	normal = normalize(matrix * vec4(norm, 1)).xyz;\n"
"}\n";

const char* fs =
"#version 430\n"
"layout(location=0) in vec2 uv;\n"
"layout(location=1) in vec3 vertPos;\n"
"layout(location=2) in vec3 normal;\n"
"out vec3 Color;\n"
"void main()\n"
"{\n"
"	Color = abs(normal);\n"
"}\n";

void Shader_error_log(const char* msg, GLuint shader)
{
	GLint log_length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 1)
	{
		GLchar* buf = new GLchar[log_length];
		glGetShaderInfoLog(shader, log_length, NULL, buf);
		printf("%s %s %d\n", msg, buf, log_length);
		delete[] buf;
	}
}

void Program_error_log(const char* msg, GLuint program)
{
	GLint log_length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 1)
	{
		GLchar* buf = new GLchar[log_length];
		glGetProgramInfoLog(program, log_length, NULL, buf);
		printf("%s %s \n", msg, buf);
		delete[] buf;
	}
}

void Set_matrix_rot(float radians, float (&matrix)[16])
{
	float mCos = cosf(radians);
	float mSin = sinf(radians);

	matrix[0] = mCos;
	matrix[2] = mSin;
	matrix[8] = -mSin;
	matrix[10] = mCos;
}

bool Setup()
{
	// Initialize GLFW and create a window
	window = Window::Create_window(640, 640, "Triangle_app");
	if (window == nullptr)
	{
		printf("Window: Failed to create window\nEnter any button to continue ");
		scanf("%*s");
		return false;
	}
	
	glEnable(GL_CULL_FACE);
	
	// Setup vertex shader
	vs_int = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs_int, 1, &vs, NULL);
	glCompileShader(vs_int);
	Shader_error_log("[VERTEX SHADER COMPILE ERROR]:", vs_int);

	// Setup fragment shader
	fs_int = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs_int, 1, &fs, NULL);
	glCompileShader(fs_int);
	Shader_error_log("[FRAGMENT SHADER COMPILE ERROR]:", fs_int);

	// Create a program object
	program = glCreateProgram();
	glAttachShader(program, vs_int);
	glAttachShader(program, fs_int);
	glLinkProgram(program);
	Program_error_log("[PROGRAM LINK ERROR]:", program);
	
	// Set clear color to gray
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Create mesh structure and mesh resource
	std::vector<float> vbo;
	std::vector<int> ebo;
	meshData = HE_mesh_V3::LoadFromOBJ("../source/from_school/Resources/cube.obj");
	meshData->ToMeshResource(&vbo, &ebo);
	myMesh = new MeshResource(true, true);
	myMesh->SetUpVBO(&vbo[0], vbo.size());
	myMesh->SetUpEBO(&ebo[0], ebo.size());

	// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

	return true;
}

void Application_loop()
{
	glUseProgram(program);
	
	int subdiv_counter = 1;
	bool wireframe = false;
	float rot = 0;
	float rot_matrix[] = 
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 0
	};
	
	// //Program loop
	while (!glfwWindowShouldClose(window))
	{
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		// UI
		ImGui::Begin("Controls"); 
		if (ImGui::Button("Subdivide"))
		{
			// Subdivide
			std::chrono::time_point<std::chrono::system_clock> start;
			start = std::chrono::system_clock::now();
			meshData->Subdivide();
			std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - start;
			float time = elapsed_seconds.count() * 1000;
			printf("Subdivision number: %d Time taken (ms): %f\n", subdiv_counter, time);
			subdiv_counter++;

			// Get new mesh data
			std::vector<float> vbo;
			std::vector<int> ebo;
			meshData->ToMeshResource(&vbo, &ebo);

			// Replace mesh
			delete myMesh;
			myMesh = new MeshResource(true, true);
			myMesh->SetUpVBO(&vbo[0], vbo.size());
			myMesh->SetUpEBO(&ebo[0], ebo.size());
		}
		if (ImGui::Button("Wireframe"))
		{
			wireframe = !wireframe;
		}
        ImGui::End();
		
		// Rotate then draw
		rot += 0.01f;
		Set_matrix_rot(rot, rot_matrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "matrix"), 1, GL_FALSE, rot_matrix);
		myMesh->Draw(wireframe);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void UI_loop()
{

}

int main()
{
	if (Setup() == true)
		Application_loop();
	Window::Destroy_window(window);
	return 0;
}