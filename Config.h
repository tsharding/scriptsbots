#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector> // Added for getKeys()

class Config {
private:
    std::map<std::string, std::string> configValues;
    std::string configFile;
    
    void parseLine(const std::string& line);
    std::string trim(const std::string& str);
    bool isComment(const std::string& line);
    bool isSection(const std::string& line);
    
public:
    Config(const std::string& filename = "scriptbots.conf");
    ~Config();
    
    bool load();
    bool save();
    
    // Getter methods with type conversion
    int getInt(const std::string& key, int defaultValue = 0) const;
    float getFloat(const std::string& key, float defaultValue = 0.0f) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    
    // Setter methods
    void setInt(const std::string& key, int value);
    void setFloat(const std::string& key, float value);
    void setBool(const std::string& key, bool value);
    void setString(const std::string& key, const std::string& value);
    
    // Check if key exists
    bool hasKey(const std::string& key) const;
    
    // Get all keys
    std::vector<std::string> getKeys() const;
    
    // Cache configuration values for performance
    void cacheValues();
    
    // Restore cached values from a save file (for load compatibility)
    void restoreFromSaveFile(std::ifstream& file);
};

// Global configuration instance
extern Config g_config;

// Cached configuration values for performance
namespace conf_cache {
    // World dimensions
    extern int WIDTH;
    extern int HEIGHT;
    extern int WWIDTH;
    extern int WHEIGHT;
    extern int CZ;
    
    // Agent settings
    extern int NUMBOTS;
    extern float BOTRADIUS;
    extern float BOTSPEED;
    extern float SPIKESPEED;
    extern float SPIKEMULT;
    extern int BABIES;
    extern float BOOSTSIZEMULT;
    extern float REPRATEH;
    extern float REPRATEC;
    
    // Vision and perception
    extern float DIST;
    extern float METAMUTRATE1;
    extern float METAMUTRATE2;
    
    // Food system
    extern float FOODINTAKE;
    extern float FOODWASTE;
    extern float FOODMAX;
    extern int FOODADDFREQ;
    extern float FOODTRANSFER;
    extern float FOOD_SHARING_DISTANCE;
    extern float FOOD_DISTRIBUTION_RADIUS;
    extern float REPMULT;
    
    // Simulation settings
    extern int AUTOSAVE_FREQUENCY;
    extern int RANDOM_SPAWN_EPOCH_INTERVAL;
    extern int RANDOM_SPAWN_COUNT;
    extern bool INITIAL_CLOSED_ENVIRONMENT;
    
    // Neural network settings
    extern int INPUTSIZE;
    extern int OUTPUTSIZE;
    extern int NUMEYES;
    extern int BRAINSIZE;
    extern int CONNS;
}

// Configuration access functions (to replace conf:: namespace)
namespace conf {
    // World dimensions
    inline int WIDTH() { return conf_cache::WIDTH; }
    inline int HEIGHT() { return conf_cache::HEIGHT; }
    inline int WWIDTH() { return conf_cache::WWIDTH; }
    inline int WHEIGHT() { return conf_cache::WHEIGHT; }
    inline int CZ() { return conf_cache::CZ; }
    
    // Agent settings
    inline int NUMBOTS() { return conf_cache::NUMBOTS; }
    inline float BOTRADIUS() { return conf_cache::BOTRADIUS; }
    inline float BOTSPEED() { return conf_cache::BOTSPEED; }
    inline float SPIKESPEED() { return conf_cache::SPIKESPEED; }
    inline float SPIKEMULT() { return conf_cache::SPIKEMULT; }
    inline int BABIES() { return conf_cache::BABIES; }
    inline float BOOSTSIZEMULT() { return conf_cache::BOOSTSIZEMULT; }
    inline float REPRATEH() { return conf_cache::REPRATEH; }
    inline float REPRATEC() { return conf_cache::REPRATEC; }
    
    // Vision and perception
    inline float DIST() { return conf_cache::DIST; }
    inline float METAMUTRATE1() { return conf_cache::METAMUTRATE1; }
    inline float METAMUTRATE2() { return conf_cache::METAMUTRATE2; }
    
    // Food system
    inline float FOODINTAKE() { return conf_cache::FOODINTAKE; }
    inline float FOODWASTE() { return conf_cache::FOODWASTE; }
    inline float FOODMAX() { return conf_cache::FOODMAX; }
    inline int FOODADDFREQ() { return conf_cache::FOODADDFREQ; }
    inline float FOODTRANSFER() { return conf_cache::FOODTRANSFER; }
    inline float FOOD_SHARING_DISTANCE() { return conf_cache::FOOD_SHARING_DISTANCE; }
    inline float FOOD_DISTRIBUTION_RADIUS() { return conf_cache::FOOD_DISTRIBUTION_RADIUS; }
    inline float REPMULT() { return conf_cache::REPMULT; }
    
    // Simulation settings
    inline int AUTOSAVE_FREQUENCY() { return conf_cache::AUTOSAVE_FREQUENCY; }
    inline int RANDOM_SPAWN_EPOCH_INTERVAL() { return conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL; }
    inline int RANDOM_SPAWN_COUNT() { return conf_cache::RANDOM_SPAWN_COUNT; }
    inline bool INITIAL_CLOSED_ENVIRONMENT() { return conf_cache::INITIAL_CLOSED_ENVIRONMENT; }
    
    // Neural network settings
    inline int INPUTSIZE() { return conf_cache::INPUTSIZE; }
    inline int OUTPUTSIZE() { return conf_cache::OUTPUTSIZE; }
    inline int NUMEYES() { return conf_cache::NUMEYES; }
    inline int BRAINSIZE() { return conf_cache::BRAINSIZE; }
    inline int CONNS() { return conf_cache::CONNS; }
}

#endif 