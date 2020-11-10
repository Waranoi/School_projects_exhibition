//
// Created by eliwar-5 on 2017-10-31.
//

#ifndef GSCEPT_LAB_ENV_APPLICATION_H
#define GSCEPT_LAB_ENV_APPLICATION_H

#include "core/app.h"
#include "render/window.h"
#include "config.h"
#include <fstream>
#include <string>
#include <assert.h>

class Application : public Core::App
{
public:
    Application();
    ~Application();
    bool Open();
    void Run();
    std::string GetShaderText(const char* file);
    void ShaderErrorLog(GLuint shader, const char* type = "");
    void ProgramErrorLog(GLuint program);
private:

    GLuint program;
    GLuint vsIndex;
    GLuint fsIndex;
    GLuint vbo;
    Display::Window* window;
};


#endif //GSCEPT_LAB_ENV_APPLICATION_H
