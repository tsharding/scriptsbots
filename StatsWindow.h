#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include "World.h"
#include <GL/glut.h>

class StatsWindow
{
public:
    StatsWindow(World* w);
    ~StatsWindow();
    
    void setWorld(World* w);
    
    // GLUT callback functions
    static void gl_renderStatsScene();
    static void gl_changeStatsSize(int w, int h);
    static void gl_processStatsKeys(unsigned char key, int x, int y);
    
    void renderScene();
    void changeSize(int w, int h);
    void processKeys(unsigned char key, int x, int y);
    
    // Window management
    static void createWindow();
    static int getWindowId() { return statsWindowId; }
    
    // Update function
    void updateDisplay();
    
private:
    World* world;
    static int statsWindowId;
    static StatsWindow* instance;
    
    void drawPopulationChart();
    void drawControlsInfo();
    void drawStatsInfo();
};

extern StatsWindow* STATSWINDOW;

#endif // STATSWINDOW_H 