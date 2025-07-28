#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include "World.h"
#include <GL/glut.h>
#include <map>
#include <vector>
#include <string>

// Structure to hold lineage statistics
struct LineageStats {
    std::string tag;
    int currentPopulation;
    int maxPopulation;
    float averageGeneration;
    int currentMaxAge;
    int allTimeMaxAge;
    
    LineageStats() : currentPopulation(0), maxPopulation(0), averageGeneration(0.0f), currentMaxAge(0), allTimeMaxAge(0) {}
};

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
    static void createWindow(int x = -1, int y = -1);
    static int getWindowId() { return statsWindowId; }
    
    // Update function
    void updateDisplay();
    
private:
    World* world;
    static int statsWindowId;
    static StatsWindow* instance;
    float currentMaxScale; // Track the current maximum scale
    
    // Track all-time maximum population for each lineage
    static std::map<std::string, int> lineageMaxPopulations;
    // Track all-time maximum age for each lineage
    static std::map<std::string, int> lineageMaxAges;
    
    void drawPopulationChart();
    void drawControlsInfo();
    void drawStatsInfo();
    void drawLineageStats(); // New method for lineage statistics
    
    // Helper methods for lineage statistics
    std::vector<LineageStats> calculateTopLineages(int count = 5);
    void drawLineageTable(const std::vector<LineageStats>& lineages, int startX, int startY);
};

extern StatsWindow* STATSWINDOW;

#endif // STATSWINDOW_H 