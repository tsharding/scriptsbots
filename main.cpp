#include "GLView.h"
#include "StatsWindow.h"
#include "World.h"

#include "config.h"
#ifdef LOCAL_GLUT32
    #include "glut.h"
#else
    #include <GL/glut.h>
#endif

#include <stdio.h>


GLView* GLVIEW = new GLView(0);
StatsWindow* STATSWINDOW = nullptr;

int main(int argc, char **argv) {
    srand(time(0));
    if (conf::WIDTH%conf::CZ!=0 || conf::HEIGHT%conf::CZ!=0) printf("CAREFUL! The cell size variable conf::CZ should divide evenly into  both conf::WIDTH and conf::HEIGHT! It doesn't right now!");
    
    printf("ScriptBots now runs in two windows:\n");
    printf("- Main window: World visualization\n");
    printf("- Stats window: Population chart and controls\n");
    printf("Both windows respond to keyboard input.\n");
    
    World* world = new World();
    GLVIEW->setWorld(world);

    //GLUT SETUP
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    
    // Create main world window
    glutInitWindowPosition(30,30);
    glutInitWindowSize(conf::WWIDTH,conf::WHEIGHT);
    int mainWindowId = glutCreateWindow("ScriptBots - World View");
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glutDisplayFunc(gl_renderScene);
    glutIdleFunc(gl_handleIdle);
    glutReshapeFunc(gl_changeSize);
    glutKeyboardFunc(gl_processNormalKeys);
    glutMouseFunc(gl_processMouse);
    glutMotionFunc(gl_processMouseActiveMotion);

    // Create stats window
    STATSWINDOW = new StatsWindow(world);
    StatsWindow::createWindow();
    
    // Set the main window as the current window
    glutSetWindow(mainWindowId);

    glutMainLoop();
    return 0;
}
