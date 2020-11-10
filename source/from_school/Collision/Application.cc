//
// Created by eliwar-5 on 2017-10-31.
//

#include "Application.h"

Application::Application()
{

}

Application::~Application()
{

}

bool Application::Open()
{
    App::Open();
    this->window = new Display::Window;
    window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
                                {
                                    if (action == GLFW_REPEAT)
                                        return;

                                    switch (key)
                                    {
                                        case GLFW_KEY_ESCAPE:
                                            if (action == GLFW_PRESS)
                                                this->window->Close();
                                            break;
                                        case GLFW_KEY_W:
                                            if (action == GLFW_PRESS);
                                            break;
                                        case GLFW_KEY_S:
                                            if (action == GLFW_PRESS);
                                            break;
                                        case GLFW_KEY_A:
                                            if (action == GLFW_PRESS);
                                            break;
                                        case GLFW_KEY_D:
                                            if (action == GLFW_PRESS);
                                            break;
                                    }
                                });

    float buf[] =
            {
                    -0.5f, -0.5f, 0,
                    -0.5f, 0.5f, 0,
                    0.5f, 0.5f, 0,

                    -0.5f, -0.5f, 0,
                    0.5f, 0.5f, 0,
                    0.5f, -0.5f, 0
            };

    if (this->window->Open())
    {
        //Set clear color to gray
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        //Varför kan jag inte göra *vsChar = GetShaderText().c_str()? Returnerar inte GetShaderText en std::string kopia? Varför får jag en dangling pointer om jag gör så?
        std::string vsString = GetShaderText("resources/simpleVS");
        std::string fsString = GetShaderText("resources/simpleFS");
        const char *vsChar = vsString.c_str();
        const char *fsChar = fsString.c_str();

        //Setup vertex shader
        vsIndex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vsIndex, 1, &vsChar, NULL);
        glCompileShader(vsIndex);
        ShaderErrorLog(vsIndex, "VERTEX SHADER");

        //Setup fragment shader
        fsIndex = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsIndex, 1, &fsChar, NULL);
        glCompileShader(fsIndex);
        ShaderErrorLog(vsIndex, "FRAGMENT SHADER");

        program = glCreateProgram();
        glAttachShader(program, vsIndex);
        glAttachShader(program, fsIndex);
        glLinkProgram(program);
        ProgramErrorLog(program);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return true;
    }
    return false;
}

void Application::Run()
{
    while (this->window->IsOpen())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        this->window->Update();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glUseProgram(program);
        glEnableVertexAttribArray(0);
        //glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 3, NULL);
        //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        this->window->SwapBuffers();
    }
}

std::string Application::GetShaderText(const char *file)
{
    std::string content;
    std::ifstream fileStream(file, std::ios::in);

    //Open file stream, stop execution if file can not be opened
    assert(fileStream.is_open() && "File not found");

    //Append lines from file stream to std::string content
    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    //Close file stream
    fileStream.close();

    //Return const char* of text found in file
    return content;
}

void Application::ShaderErrorLog(GLuint shader, const char* type)
{
    int logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar* buf = new GLchar[logLength];
        glGetShaderInfoLog(shader, logLength, NULL, buf);
        printf("[%s COMPILE ERROR]: %s\n\n", type, buf);
        delete[] buf;
    }
}

void Application::ProgramErrorLog(GLuint program)
{
    int logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar* buf = new GLchar[logLength];
        glGetProgramInfoLog(program, logLength, NULL, buf);
        printf("[PROGRAM LINK ERROR]: %s", buf);
        delete[] buf;
    }
}