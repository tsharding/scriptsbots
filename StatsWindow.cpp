#include "StatsWindow.h"
#include "GLView.h"
#include "settings.h"
#include <stdio.h>
#include <string.h>

// Forward declaration of RenderString function (defined in GLView.cpp)
void RenderString(float x, float y, void *font, const char* string, float r, float g, float b);

// Helper function to calculate text width for right-justification
int getTextWidth(const char* string, void* font) {
    int width = 0;
    int len = (int) strlen(string);
    for (int i = 0; i < len; i++) {
        width += glutBitmapWidth(font, string[i]);
    }
    return width;
}

// Global instance (declared in main.cpp)
extern StatsWindow* STATSWINDOW;
int StatsWindow::statsWindowId = -1;
StatsWindow* StatsWindow::instance = nullptr;

StatsWindow::StatsWindow(World* w) : world(w), currentMaxScale(10.0f)
{
    instance = this;
}

StatsWindow::~StatsWindow()
{
    instance = nullptr;
}

void StatsWindow::setWorld(World* w)
{
    world = w;
}

void StatsWindow::updateDisplay()
{
    if (statsWindowId > 0) {
        int currentWindow = glutGetWindow();
        glutSetWindow(statsWindowId);
        glutPostRedisplay();
        if (currentWindow != statsWindowId) {
            glutSetWindow(currentWindow);
        }
    }
}

void StatsWindow::createWindow(int x, int y)
{
    // Use provided position or calculate default position
    if (x == -1 || y == -1) {
        // Default positioning (original behavior)
        glutInitWindowPosition(conf::WWIDTH() + 50, 30);
    } else {
        glutInitWindowPosition(x, y);
    }
    
    glutInitWindowSize(500, 800);
    statsWindowId = glutCreateWindow("ScriptBots - Stats & Controls");
    glClearColor(0.95f, 0.95f, 0.95f, 0.0f);
    
    glutDisplayFunc(gl_renderStatsScene);
    glutReshapeFunc(gl_changeStatsSize);
    glutKeyboardFunc(gl_processStatsKeys);
    // Don't set mouse functions - stats window should not handle mouse events
}

void StatsWindow::gl_renderStatsScene()
{
    // Only render if this is the stats window
    if (glutGetWindow() == statsWindowId && instance) {
        instance->renderScene();
    }
}

void StatsWindow::gl_changeStatsSize(int w, int h)
{
    if (instance) {
        instance->changeSize(w, h);
    }
}

void StatsWindow::gl_processStatsKeys(unsigned char key, int x, int y)
{
    if (instance) {
        instance->processKeys(key, x, y);
    }
}

void StatsWindow::changeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void StatsWindow::processKeys(unsigned char key, int x, int y)
{
    // Handle key events directly in stats window without switching context
    // This prevents interference with main window rendering
    if (GLVIEW) {
        // Call the main window's key handler directly without window switching
        // The main window's key handler now works regardless of window focus
        GLVIEW->processNormalKeys(key, x, y);
    }
}

void StatsWindow::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    drawPopulationChart();
    drawStatsInfo();
    drawControlsInfo();
    
    glutSwapBuffers();
}

void StatsWindow::drawPopulationChart()
{
    if (!world) return;
    
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    
    // Draw chart background
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.9f);
    glVertex2f(20, 20);
    glVertex2f(windowWidth - 20, 20);
    glVertex2f(windowWidth - 20, 200);
    glVertex2f(20, 200);
    glEnd();
    
    // Draw chart border
    glBegin(GL_LINE_LOOP);
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex2f(20, 20);
    glVertex2f(windowWidth - 20, 20);
    glVertex2f(windowWidth - 20, 200);
    glVertex2f(20, 200);
    glEnd();
    
    // Draw population lines
    float chartWidth = windowWidth - 60;
    float chartHeight = 160;
    
    // Calculate current maximum in the displayed range
    float maxPop = 1.0f; // Minimum scale to avoid division by zero
    for(int q = 0; q < world->numHerbivore.size(); q++) {
        if(world->numHerbivore[q] > maxPop) maxPop = world->numHerbivore[q];
        if(world->numCarnivore[q] > maxPop) maxPop = world->numCarnivore[q];
    }
    // Add some padding to the scale
    maxPop = maxPop * 1.1f;
    if(maxPop < 10.0f) maxPop = 10.0f; // Minimum scale for visibility
    
    // Update the scale to fit the current data range
    // If the new maximum exceeds the current scale, increase it
    // If the new maximum is below the current scale, decrease it
    currentMaxScale = maxPop;
    
    // Use the current scale (which now adapts both up and down)
    maxPop = currentMaxScale;
    
    glBegin(GL_LINES);
    
    // Herbivore line (green)
    glColor3f(0, 1, 0);
    for(int q = 0; q < world->numHerbivore.size() - 1; q++) {
        float x1 = 30 + (q * chartWidth) / world->numHerbivore.size();
        float y1 = 180 - (world->numHerbivore[q] * chartHeight) / maxPop;
        float x2 = 30 + ((q + 1) * chartWidth) / world->numHerbivore.size();
        float y2 = 180 - (world->numHerbivore[q + 1] * chartHeight) / maxPop;
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }
    
    // Carnivore line (red)
    glColor3f(1, 0, 0);
    for(int q = 0; q < world->numCarnivore.size() - 1; q++) {
        float x1 = 30 + (q * chartWidth) / world->numCarnivore.size();
        float y1 = 180 - (world->numCarnivore[q] * chartHeight) / maxPop;
        float x2 = 30 + ((q + 1) * chartWidth) / world->numCarnivore.size();
        float y2 = 180 - (world->numCarnivore[q + 1] * chartHeight) / maxPop;
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }
    
    // Current time indicator (black line) - now always at the right edge
    glColor3f(0, 0, 0);
    float currentX = 30 + chartWidth;
    glVertex2f(currentX, 20);
    glVertex2f(currentX, 200);
    
    glEnd();
    
    // Draw chart title
    char buf[256];
    sprintf(buf, "Population Chart (Green=Herbivores, Red=Carnivores)");
    RenderString(30, 15, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    
    // Draw Y-axis labels - simple positioning with gap from plot edge
    const int labelX = 25; // Fixed X position for all labels
    
    // Max label - positioned to avoid being cut off at top
    sprintf(buf, "Max: %.0f", maxPop);
    RenderString(labelX, 35, GLUT_BITMAP_HELVETICA_10, buf, 0.0f, 0.0f, 0.0f);
    
    // 75% label
    sprintf(buf, "%.0f", maxPop * 0.75f);
    RenderString(labelX, 70, GLUT_BITMAP_HELVETICA_10, buf, 0.0f, 0.0f, 0.0f);
    
    // 50% label
    sprintf(buf, "%.0f", maxPop * 0.5f);
    RenderString(labelX, 110, GLUT_BITMAP_HELVETICA_10, buf, 0.0f, 0.0f, 0.0f);
    
    // 25% label
    sprintf(buf, "%.0f", maxPop * 0.25f);
    RenderString(labelX, 150, GLUT_BITMAP_HELVETICA_10, buf, 0.0f, 0.0f, 0.0f);
    
    // 0 label
    sprintf(buf, "0");
    RenderString(labelX, 190, GLUT_BITMAP_HELVETICA_10, buf, 0.0f, 0.0f, 0.0f);
}

void StatsWindow::drawStatsInfo()
{
    if (!world) return;
    
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int yPos = 220;
    char buf[256];
    
    // Current statistics
    std::pair<int, int> num_herbs_carns = world->numHerbCarnivores();
    
    sprintf(buf, "Current Statistics:");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 20;
    
    sprintf(buf, "Total Agents: %d", world->numAgents());
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "Herbivores: %d", num_herbs_carns.first);
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.8f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "Carnivores: %d", num_herbs_carns.second);
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.8f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "Epoch: %d", world->epoch());
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "Total Food: %.1f", world->getTotalFood());
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "Food Tiles: %.2f%%", world->getFoodTilePercentage());
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    // Display current FPS
    if (GLVIEW) {
        sprintf(buf, "Current FPS: %d", GLVIEW->getCurrentFPS());
        RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.8f);
        yPos += 15;
    }
    
    yPos += 5;
}

void StatsWindow::drawControlsInfo()
{
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int yPos = 350;
    char buf[256];
    
    sprintf(buf, "Controls:");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 20;
    
    sprintf(buf, "ESC - Exit simulation");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "p - Pause/Unpause");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "d - Toggle drawing (speed)");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "f - Toggle food display");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "g - Toggle agent info (text & health bars)");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "v - Save simulation state");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "l - Load simulation state");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "+/- - Speed control");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "r - Reset all agents");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "a - Add 10 crossover agents");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "q - Add 10 carnivores");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "h - Add 10 herbivores");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "c - Toggle closed environment");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "s - Follow strongest agent");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "o - Follow oldest agent");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "j - Recenter camera view");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 20;
    
    sprintf(buf, "Mouse Controls:");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 20;
    
    sprintf(buf, "Left Click - Select agent");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "Right Drag - Pan camera");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "Middle Drag - Fine zoom");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
    yPos += 15;
    
    sprintf(buf, "Mouse Wheel - Zoom in/out");
    RenderString(20, yPos, GLUT_BITMAP_HELVETICA_12, buf, 0.0f, 0.0f, 0.0f);
} 