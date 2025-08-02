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
    int totalPopulation;  // Total cumulative population for this lineage
    float averageGeneration;
    int currentMaxAge;
    int allTimeMaxAge;
    int emergenceEpoch;     // When the lineage first appeared (epoch)
    int emergenceTick;      // When the lineage first appeared (tick within epoch)
    int extinctionEpoch;    // When the lineage went extinct (epoch, 0 if still alive)
    int extinctionTick;     // When the lineage went extinct (tick within epoch, 0 if still alive)
    int maxGeneration;      // Maximum generation for any agent in this lineage
    
    LineageStats() : currentPopulation(0), maxPopulation(0), totalPopulation(0), averageGeneration(0.0f), currentMaxAge(0), allTimeMaxAge(0), emergenceEpoch(0), emergenceTick(0), extinctionEpoch(0), extinctionTick(0), maxGeneration(0) {}
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
    
    // Static function to track agent creation for lineage statistics
    static void trackAgentCreation(const std::string& lineageTag, int generation = 0, float currentEpoch = 0.0f, int currentTick = 0);
    
    // Static function to track lineage extinction
    static void trackLineageExtinction(const std::string& lineageTag, float currentEpoch, int currentTick);
    
private:
    World* world;
    static int statsWindowId;
    static StatsWindow* instance;
    float currentMaxScale; // Track the current maximum scale
    
    // Track all-time maximum population for each lineage
    static std::map<std::string, int> lineageMaxPopulations;
    // Track all-time maximum age for each lineage
    static std::map<std::string, int> lineageMaxAges;
    // Track total cumulative population for each lineage
    static std::map<std::string, int> lineageTotalPopulations;
    // Track emergence epoch for each lineage
    static std::map<std::string, float> lineageEmergenceEpochs;
    // Track extinction epoch for each lineage (0 if still alive)
    static std::map<std::string, float> lineageExtinctionEpochs;
    // Track maximum generation for each lineage
    static std::map<std::string, int> lineageMaxGenerations;
    
    // Persistent list of top 20 lineages from entire simulation
    static std::vector<LineageStats> persistentTopLineages;
    
    // Track emergence time for each lineage (epoch and tick)
    static std::map<std::string, std::pair<int, int>> lineageEmergenceTimes;
    // Track extinction time for each lineage (epoch and tick, 0,0 if still alive)
    static std::map<std::string, std::pair<int, int>> lineageExtinctionTimes;
    
    void drawPopulationChart();
    void drawControlsInfo();
    void drawStatsInfo();
    void drawLineageStats(); // New method for lineage statistics
    
    // Helper methods for lineage statistics
    std::vector<LineageStats> calculateTopLineages(int count = 5);
    std::vector<LineageStats> calculateHallOfFameLineages(int count = 20);
    static void updatePersistentTopLineages();
    void drawLineageTable(const std::vector<LineageStats>& lineages, int startX, int startY);
    void drawHallOfFameTable(const std::vector<LineageStats>& lineages, int startX, int startY);
};

extern StatsWindow* STATSWINDOW;

#endif // STATSWINDOW_H 