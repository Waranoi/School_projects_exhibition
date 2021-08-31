//
// Created by eliwar-5 on 2017-10-31.
//

#include "exampleapp.h"
#include "ForceApp.h"
#include "CollisionApp.h"
#include "Application.h"
#include <GL/glut.h>

int
main(int argc, char** argv)
{
    glutInit(&argc, argv);
    Example::ExampleApp app;
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}