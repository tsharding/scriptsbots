#include "Config.h"
#include <algorithm>
#include <cctype>

// Global configuration instance
Config g_config;

// Cached configuration values for performance
namespace conf_cache {
    // World dimensions
    int WIDTH = 9000;
    int HEIGHT = 6000;
    int WWIDTH = 1600;
    int WHEIGHT = 900;
    int CZ = 50;
    
    // Agent settings
    int NUMBOTS = 70;
    float BOTRADIUS = 10.0f;
    float BOTSPEED = 0.3f;
    float SPIKESPEED = 0.005f;
    float SPIKEMULT = 1.0f;
    int BABIES = 2;
    float BOOSTSIZEMULT = 2.0f;
    float REPRATEH = 7.0f;
    float REPRATEC = 7.0f;
    
    // Vision and perception
    float DIST = 150.0f;
    float METAMUTRATE1 = 0.002f;
    float METAMUTRATE2 = 0.05f;
    
    // Food system
    float FOODINTAKE = 0.002f;
    float FOODWASTE = 0.001f;
    float FOODMAX = 1.5f;
    int FOODADDFREQ = 50;
    float FOODTRANSFER = 0.001f;
    float FOOD_SHARING_DISTANCE = 50.0f;
    float FOOD_DISTRIBUTION_RADIUS = 100.0f;
    float REPMULT = 5.0f;
    float PROP_INIT_FOOD_FILLED = 0.1f;
    
    // Simulation settings
    int AUTOSAVE_FREQUENCY = 100;
    int RANDOM_SPAWN_EPOCH_INTERVAL = 5;
    int RANDOM_SPAWN_COUNT = 5;
    bool INITIAL_CLOSED_ENVIRONMENT = true;
    int HERBIVORE_EXTINCTION_REPOPULATION_COUNT = 25;
    int CARNIVORE_EXTINCTION_REPOPULATION_COUNT = 25;
    
    // Neural network settings
    int INPUTSIZE = 24;
    int OUTPUTSIZE = 9;
    int NUMEYES = 4;
    int BRAINSIZE = 200;
    int CONNS = 4;
}

Config::Config(const std::string& filename) : configFile(filename) {
}

Config::~Config() {
}

bool Config::load() {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file '" << configFile << "'. Using default values." << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        parseLine(line);
    }
    
    file.close();
    
    // Cache the values after loading
    cacheValues();
    
    return true;
}

void Config::cacheValues() {
    // World dimensions
    conf_cache::WIDTH = getInt("world.width", 9000);
    conf_cache::HEIGHT = getInt("world.height", 6000);
    conf_cache::WWIDTH = getInt("world.window_width", 1600);
    conf_cache::WHEIGHT = getInt("world.window_height", 900);
    conf_cache::CZ = getInt("world.cell_size", 50);
    
    // Agent settings
    conf_cache::NUMBOTS = getInt("agent.initial_count", 70);
    conf_cache::BOTRADIUS = getFloat("agent.radius", 10.0f);
    conf_cache::BOTSPEED = getFloat("agent.speed", 0.3f);
    conf_cache::SPIKESPEED = getFloat("agent.spike_speed", 0.005f);
    conf_cache::SPIKEMULT = getFloat("agent.spike_multiplier", 1.0f);
    conf_cache::BABIES = getInt("agent.babies_per_reproduction", 2);
    conf_cache::BOOSTSIZEMULT = getFloat("agent.boost_multiplier", 2.0f);
    conf_cache::REPRATEH = getFloat("agent.herbivore_reproduction_rate", 7.0f);
    conf_cache::REPRATEC = getFloat("agent.carnivore_reproduction_rate", 7.0f);
    
    // Vision and perception
    conf_cache::DIST = getFloat("perception.vision_distance", 150.0f);
    conf_cache::METAMUTRATE1 = getFloat("evolution.meta_mutation_rate_1", 0.002f);
    conf_cache::METAMUTRATE2 = getFloat("evolution.meta_mutation_rate_2", 0.05f);
    
    // Food system
    conf_cache::FOODINTAKE = getFloat("food.intake_rate", 0.002f);
    conf_cache::FOODWASTE = getFloat("food.waste_rate", 0.001f);
    conf_cache::FOODMAX = getFloat("food.max_per_cell", 1.5f);
    conf_cache::FOODADDFREQ = getInt("food.add_frequency", 50);
    conf_cache::FOODTRANSFER = getFloat("food.transfer_rate", 0.001f);
    conf_cache::FOOD_SHARING_DISTANCE = getFloat("food.sharing_distance", 50.0f);
    conf_cache::FOOD_DISTRIBUTION_RADIUS = getFloat("food.distribution_radius", 100.0f);
    conf_cache::REPMULT = getFloat("evolution.reproduction_multiplier", 5.0f);
    conf_cache::PROP_INIT_FOOD_FILLED = getFloat("food.initial_fill_proportion", 0.1f);
    
    // Simulation settings
    conf_cache::AUTOSAVE_FREQUENCY = getInt("simulation.autosave_frequency", 100);
    conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL = getInt("simulation.random_spawn_epoch_interval", 5);
    conf_cache::RANDOM_SPAWN_COUNT = getInt("simulation.random_spawn_count", 5);
    conf_cache::INITIAL_CLOSED_ENVIRONMENT = getBool("simulation.initial_closed_environment", true);
    conf_cache::HERBIVORE_EXTINCTION_REPOPULATION_COUNT = getInt("simulation.herbivore_extinction_repopulation_count", 25);
    conf_cache::CARNIVORE_EXTINCTION_REPOPULATION_COUNT = getInt("simulation.carnivore_extinction_repopulation_count", 25);
    
    // Neural network settings
    conf_cache::INPUTSIZE = getInt("neural.input_size", 24);
    conf_cache::OUTPUTSIZE = getInt("neural.output_size", 9);
    conf_cache::NUMEYES = getInt("neural.num_eyes", 4);
    conf_cache::BRAINSIZE = getInt("neural.brain_size", 200);
    conf_cache::CONNS = getInt("neural.connections_per_neuron", 4);
}

void Config::restoreFromSaveFile(std::ifstream& file) {
    // Read configuration values from save file and restore to cache
    // World dimensions
    file.read(reinterpret_cast<char*>(&conf_cache::WIDTH), sizeof(conf_cache::WIDTH));
    file.read(reinterpret_cast<char*>(&conf_cache::HEIGHT), sizeof(conf_cache::HEIGHT));
    file.read(reinterpret_cast<char*>(&conf_cache::WWIDTH), sizeof(conf_cache::WWIDTH));
    file.read(reinterpret_cast<char*>(&conf_cache::WHEIGHT), sizeof(conf_cache::WHEIGHT));
    file.read(reinterpret_cast<char*>(&conf_cache::CZ), sizeof(conf_cache::CZ));
    
    // Agent settings
    file.read(reinterpret_cast<char*>(&conf_cache::NUMBOTS), sizeof(conf_cache::NUMBOTS));
    file.read(reinterpret_cast<char*>(&conf_cache::BOTRADIUS), sizeof(conf_cache::BOTRADIUS));
    file.read(reinterpret_cast<char*>(&conf_cache::BOTSPEED), sizeof(conf_cache::BOTSPEED));
    file.read(reinterpret_cast<char*>(&conf_cache::SPIKESPEED), sizeof(conf_cache::SPIKESPEED));
    file.read(reinterpret_cast<char*>(&conf_cache::SPIKEMULT), sizeof(conf_cache::SPIKEMULT));
    file.read(reinterpret_cast<char*>(&conf_cache::BABIES), sizeof(conf_cache::BABIES));
    file.read(reinterpret_cast<char*>(&conf_cache::BOOSTSIZEMULT), sizeof(conf_cache::BOOSTSIZEMULT));
    file.read(reinterpret_cast<char*>(&conf_cache::REPRATEH), sizeof(conf_cache::REPRATEH));
    file.read(reinterpret_cast<char*>(&conf_cache::REPRATEC), sizeof(conf_cache::REPRATEC));
    
    // Vision and perception
    file.read(reinterpret_cast<char*>(&conf_cache::DIST), sizeof(conf_cache::DIST));
    file.read(reinterpret_cast<char*>(&conf_cache::METAMUTRATE1), sizeof(conf_cache::METAMUTRATE1));
    file.read(reinterpret_cast<char*>(&conf_cache::METAMUTRATE2), sizeof(conf_cache::METAMUTRATE2));
    
    // Food system
    file.read(reinterpret_cast<char*>(&conf_cache::FOODINTAKE), sizeof(conf_cache::FOODINTAKE));
    file.read(reinterpret_cast<char*>(&conf_cache::FOODWASTE), sizeof(conf_cache::FOODWASTE));
    file.read(reinterpret_cast<char*>(&conf_cache::FOODMAX), sizeof(conf_cache::FOODMAX));
    file.read(reinterpret_cast<char*>(&conf_cache::FOODADDFREQ), sizeof(conf_cache::FOODADDFREQ));
    file.read(reinterpret_cast<char*>(&conf_cache::FOODTRANSFER), sizeof(conf_cache::FOODTRANSFER));
    file.read(reinterpret_cast<char*>(&conf_cache::FOOD_SHARING_DISTANCE), sizeof(conf_cache::FOOD_SHARING_DISTANCE));
    file.read(reinterpret_cast<char*>(&conf_cache::FOOD_DISTRIBUTION_RADIUS), sizeof(conf_cache::FOOD_DISTRIBUTION_RADIUS));
    file.read(reinterpret_cast<char*>(&conf_cache::REPMULT), sizeof(conf_cache::REPMULT));
    file.read(reinterpret_cast<char*>(&conf_cache::PROP_INIT_FOOD_FILLED), sizeof(conf_cache::PROP_INIT_FOOD_FILLED));
    
    // Simulation settings
    file.read(reinterpret_cast<char*>(&conf_cache::AUTOSAVE_FREQUENCY), sizeof(conf_cache::AUTOSAVE_FREQUENCY));
    file.read(reinterpret_cast<char*>(&conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL), sizeof(conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL));
    file.read(reinterpret_cast<char*>(&conf_cache::RANDOM_SPAWN_COUNT), sizeof(conf_cache::RANDOM_SPAWN_COUNT));
    file.read(reinterpret_cast<char*>(&conf_cache::INITIAL_CLOSED_ENVIRONMENT), sizeof(conf_cache::INITIAL_CLOSED_ENVIRONMENT));
    file.read(reinterpret_cast<char*>(&conf_cache::HERBIVORE_EXTINCTION_REPOPULATION_COUNT), sizeof(conf_cache::HERBIVORE_EXTINCTION_REPOPULATION_COUNT));
    file.read(reinterpret_cast<char*>(&conf_cache::CARNIVORE_EXTINCTION_REPOPULATION_COUNT), sizeof(conf_cache::CARNIVORE_EXTINCTION_REPOPULATION_COUNT));
    
    // Neural network settings
    file.read(reinterpret_cast<char*>(&conf_cache::INPUTSIZE), sizeof(conf_cache::INPUTSIZE));
    file.read(reinterpret_cast<char*>(&conf_cache::OUTPUTSIZE), sizeof(conf_cache::OUTPUTSIZE));
    file.read(reinterpret_cast<char*>(&conf_cache::NUMEYES), sizeof(conf_cache::NUMEYES));
    file.read(reinterpret_cast<char*>(&conf_cache::BRAINSIZE), sizeof(conf_cache::BRAINSIZE));
    file.read(reinterpret_cast<char*>(&conf_cache::CONNS), sizeof(conf_cache::CONNS));
}

bool Config::save() {
    std::ofstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open config file '" << configFile << "' for writing." << std::endl;
        return false;
    }
    
    for (const auto& pair : configValues) {
        file << pair.first << " = " << pair.second << std::endl;
    }
    
    file.close();
    return true;
}

void Config::parseLine(const std::string& line) {
    std::string trimmed = trim(line);
    
    if (trimmed.empty() || isComment(trimmed) || isSection(trimmed)) {
        return;
    }
    
    size_t equalPos = trimmed.find('=');
    if (equalPos == std::string::npos) {
        return; // Invalid line format
    }
    
    std::string key = trim(trimmed.substr(0, equalPos));
    std::string value = trim(trimmed.substr(equalPos + 1));
    
    if (!key.empty()) {
        configValues[key] = value;
    }
}

std::string Config::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

bool Config::isComment(const std::string& line) {
    return line[0] == '#' || line[0] == ';';
}

bool Config::isSection(const std::string& line) {
    return line[0] == '[' && line[line.length() - 1] == ']';
}

int Config::getInt(const std::string& key, int defaultValue) const {
    auto it = configValues.find(key);
    if (it == configValues.end()) {
        return defaultValue;
    }
    
    try {
        return std::stoi(it->second);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

float Config::getFloat(const std::string& key, float defaultValue) const {
    auto it = configValues.find(key);
    if (it == configValues.end()) {
        return defaultValue;
    }
    
    try {
        return std::stof(it->second);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

bool Config::getBool(const std::string& key, bool defaultValue) const {
    auto it = configValues.find(key);
    if (it == configValues.end()) {
        return defaultValue;
    }
    
    std::string value = it->second;
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    
    return value == "true" || value == "1" || value == "yes" || value == "on";
}

std::string Config::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = configValues.find(key);
    if (it == configValues.end()) {
        return defaultValue;
    }
    return it->second;
}

void Config::setInt(const std::string& key, int value) {
    configValues[key] = std::to_string(value);
}

void Config::setFloat(const std::string& key, float value) {
    configValues[key] = std::to_string(value);
}

void Config::setBool(const std::string& key, bool value) {
    configValues[key] = value ? "true" : "false";
}

void Config::setString(const std::string& key, const std::string& value) {
    configValues[key] = value;
}

bool Config::hasKey(const std::string& key) const {
    return configValues.find(key) != configValues.end();
}

std::vector<std::string> Config::getKeys() const {
    std::vector<std::string> keys;
    for (const auto& pair : configValues) {
        keys.push_back(pair.first);
    }
    return keys;
} 