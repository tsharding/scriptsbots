#ifndef GLVIEW_H
#define GLVIEW_H

#include "World.h"

class GLView;

extern GLView* GLVIEW;

void gl_processNormalKeys(unsigned char key, int x, int y);
void gl_processMouse(int button, int state, int x, int y);
void gl_processMouseActiveMotion(int x, int y);
void gl_changeSize(int w, int h);
void gl_handleIdle();
void gl_renderScene();

class GLView
{

public:
    GLView(World* w);
    virtual ~GLView();
    
    void drawAgent(const Agent &a);
    void drawFood(int x, int y, float quantity);
    void drawMisc();
    
    void setWorld(World* w);
    
    // Get current FPS
    int getCurrentFPS() const;
    
    //GLUT functions
    void processNormalKeys(unsigned char key, int x, int y);
    void processMouse(int button, int state, int x, int y);
    void processMouseActiveMotion(int x, int y);
    void changeSize(int w, int h);
    void handleIdle();
    void renderScene();
    
private:
    
    World *world;
    bool paused;
    bool draw;
    int skipdraw;
    bool drawfood;
    bool showAgentInfo;
    char buf[100];
    char buf2[10];
    int modcounter;
    int lastUpdate;
    int frames;
    int currentFPS; // Store the last calculated FPS value
    
    
    float scalemult;
    float xtranslate, ytranslate;
    int downb[3];
    int mousex, mousey;
    
    int following;
    
    // Window dimensions for dynamic resizing
    int windowWidth;
    int windowHeight;
    
};

#endif // GLVIEW_H
