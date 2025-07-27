#include "GLView.h"
#include "StatsWindow.h"
#include "World.h"
#include "Config.h"

#include <GL/glut.h>

#include <stdio.h>


GLView* GLVIEW = new GLView(0);
StatsWindow* STATSWINDOW = nullptr;

int main(int argc, char **argv) {
    srand(time(0));
    
    // Load configuration
    if (!g_config.load()) {
        printf("Warning: Could not load configuration file. Using default values.\n");
        // Cache default values even if file loading failed
        g_config.cacheValues();
    }
    
    if (conf::WIDTH()%conf::CZ()!=0 || conf::HEIGHT()%conf::CZ()!=0) printf("CAREFUL! The cell size variable conf::CZ should divide evenly into both conf::WIDTH and conf::HEIGHT! It doesn't right now!");
    
    printf("ScriptBots now runs in two windows:\n");
    printf("- Main window: World visualization\n");
    printf("- Stats window: Population chart and controls\n");
    printf("Both windows respond to keyboard input.\n");
    printf("Usage: %s [--help] [--load <save_file>]\n", argv[0]);
    printf("  --help: Show this help message and exit\n");
    printf("  --load <save_file>: Load a save file from the save directory on startup (e.g., manual_save_0.sav)\n");
    
    World* world = nullptr;
    
    // Handle command-line arguments
    if (argc > 1) {
        std::string arg1 = argv[1];
        if (arg1 == "--help") {
            // Help requested
            return 0;
        } else if (arg1 == "--load") {
            if (argc < 3) {
                printf("ERROR: --load requires a filename argument.\n");
                return 1;
            }
            std::string saveFile = argv[2];
            printf("Attempting to load save file: %s\n", saveFile.c_str());
            world = new World();
            GLVIEW->setWorld(world);
            if (world->loadFromFile(saveFile)) {
                printf("Successfully loaded save file: %s\n", saveFile.c_str());
            } else {
                printf("ERROR: Failed to load save file: %s\n", saveFile.c_str());
                printf("Exiting. Please check the file name and try again.\n");
                exit(1);
            }
            // Continue to GLUT setup below
        } else {
            printf("ERROR: Unknown argument '%s'. Use --help for usage.\n", arg1.c_str());
            return 1;
        }
    } else {
        // No arguments provided, create new simulation
        world = new World();
        GLVIEW->setWorld(world);
    }
    
    //GLUT SETUP
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    
    // Create main world window
    glutInitWindowPosition(30,30);
    glutInitWindowSize(conf::WWIDTH(),conf::WHEIGHT());
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
