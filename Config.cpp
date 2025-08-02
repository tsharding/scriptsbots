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

// Configuration registration function - this is where new settings are added
void registerAllSettings() {
    // World section
    auto worldSection = std::make_shared<ConfigSection>("world", "World dimensions and display settings");
    worldSection->addSetting(std::make_shared<IntSetting>("width", "Width of the simulation world (in pixels)", 9000, &conf_cache::WIDTH));
    worldSection->addSetting(std::make_shared<IntSetting>("height", "Height of the simulation world (in pixels)", 6000, &conf_cache::HEIGHT));
    worldSection->addSetting(std::make_shared<IntSetting>("window_width", "Window width for the display (in pixels)", 1600, &conf_cache::WWIDTH));
    worldSection->addSetting(std::make_shared<IntSetting>("window_height", "Window height for the display (in pixels)", 900, &conf_cache::WHEIGHT));
    worldSection->addSetting(std::make_shared<IntSetting>("cell_size", "Cell size in pixels for food squares", 50, &conf_cache::CZ));
    g_config.addSection(worldSection);
    
    // Agent section
    auto agentSection = std::make_shared<ConfigSection>("agent", "Agent behavior and properties");
    agentSection->addSetting(std::make_shared<IntSetting>("initial_count", "Initial number of agents", 70, &conf_cache::NUMBOTS));
    agentSection->addSetting(std::make_shared<FloatSetting>("radius", "Agent radius for drawing and collision", 10.0f, &conf_cache::BOTRADIUS));
    agentSection->addSetting(std::make_shared<FloatSetting>("speed", "Base movement speed of agents", 0.3f, &conf_cache::BOTSPEED));
    agentSection->addSetting(std::make_shared<FloatSetting>("spike_speed", "How quickly attack spikes extend", 0.005f, &conf_cache::SPIKESPEED));
    agentSection->addSetting(std::make_shared<FloatSetting>("spike_multiplier", "Multiplier for spike attack strength", 1.0f, &conf_cache::SPIKEMULT));
    agentSection->addSetting(std::make_shared<IntSetting>("babies_per_reproduction", "Number of babies per reproduction", 2, &conf_cache::BABIES));
    agentSection->addSetting(std::make_shared<FloatSetting>("boost_multiplier", "Speed multiplier when boost is active", 2.0f, &conf_cache::BOOSTSIZEMULT));
    agentSection->addSetting(std::make_shared<FloatSetting>("herbivore_reproduction_rate", "Reproduction rate for herbivores", 7.0f, &conf_cache::REPRATEH));
    agentSection->addSetting(std::make_shared<FloatSetting>("carnivore_reproduction_rate", "Reproduction rate for carnivores", 7.0f, &conf_cache::REPRATEC));
    g_config.addSection(agentSection);
    
    // Perception section
    auto perceptionSection = std::make_shared<ConfigSection>("perception", "Vision and perception settings");
    perceptionSection->addSetting(std::make_shared<FloatSetting>("vision_distance", "Maximum distance agents can see", 150.0f, &conf_cache::DIST));
    g_config.addSection(perceptionSection);
    
    // Evolution section
    auto evolutionSection = std::make_shared<ConfigSection>("evolution", "Evolution and mutation parameters");
    evolutionSection->addSetting(std::make_shared<FloatSetting>("meta_mutation_rate_1", "Meta-mutation rate for mutation rate 1", 0.002f, &conf_cache::METAMUTRATE1));
    evolutionSection->addSetting(std::make_shared<FloatSetting>("meta_mutation_rate_2", "Meta-mutation rate for mutation rate 2", 0.05f, &conf_cache::METAMUTRATE2));
    evolutionSection->addSetting(std::make_shared<FloatSetting>("reproduction_multiplier", "Reproduction counter increase multiplier", 5.0f, &conf_cache::REPMULT));
    g_config.addSection(evolutionSection);
    
    // Food section
    auto foodSection = std::make_shared<ConfigSection>("food", "Food system parameters");
    foodSection->addSetting(std::make_shared<FloatSetting>("intake_rate", "Food consumption per tick", 0.002f, &conf_cache::FOODINTAKE));
    foodSection->addSetting(std::make_shared<FloatSetting>("waste_rate", "Food waste when eating", 0.001f, &conf_cache::FOODWASTE));
    foodSection->addSetting(std::make_shared<FloatSetting>("max_per_cell", "Maximum food per cell", 1.5f, &conf_cache::FOODMAX));
    foodSection->addSetting(std::make_shared<IntSetting>("add_frequency", "How often food is added", 50, &conf_cache::FOODADDFREQ));
    foodSection->addSetting(std::make_shared<FloatSetting>("transfer_rate", "Food transfer rate between agents", 0.001f, &conf_cache::FOODTRANSFER));
    foodSection->addSetting(std::make_shared<FloatSetting>("sharing_distance", "Distance for food sharing", 50.0f, &conf_cache::FOOD_SHARING_DISTANCE));
    foodSection->addSetting(std::make_shared<FloatSetting>("distribution_radius", "Radius for body distribution", 100.0f, &conf_cache::FOOD_DISTRIBUTION_RADIUS));
    foodSection->addSetting(std::make_shared<FloatSetting>("initial_fill_proportion", "Proportion of tiles with initial food", 0.1f, &conf_cache::PROP_INIT_FOOD_FILLED));
    g_config.addSection(foodSection);
    
    // Simulation section
    auto simulationSection = std::make_shared<ConfigSection>("simulation", "Simulation control parameters");
    simulationSection->addSetting(std::make_shared<IntSetting>("autosave_frequency", "Autosave frequency in epochs", 100, &conf_cache::AUTOSAVE_FREQUENCY));
    simulationSection->addSetting(std::make_shared<IntSetting>("random_spawn_epoch_interval", "Random spawn interval in epochs", 5, &conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL));
    simulationSection->addSetting(std::make_shared<IntSetting>("random_spawn_count", "Number of random agents to spawn", 5, &conf_cache::RANDOM_SPAWN_COUNT));
    simulationSection->addSetting(std::make_shared<BoolSetting>("initial_closed_environment", "Initial closed environment status", true, &conf_cache::INITIAL_CLOSED_ENVIRONMENT));
    simulationSection->addSetting(std::make_shared<IntSetting>("herbivore_extinction_repopulation_count", "Herbivores to spawn on extinction", 25, &conf_cache::HERBIVORE_EXTINCTION_REPOPULATION_COUNT));
    simulationSection->addSetting(std::make_shared<IntSetting>("carnivore_extinction_repopulation_count", "Carnivores to spawn on extinction", 25, &conf_cache::CARNIVORE_EXTINCTION_REPOPULATION_COUNT));
    g_config.addSection(simulationSection);
    
    // Neural section
    auto neuralSection = std::make_shared<ConfigSection>("neural", "Neural network parameters");
    neuralSection->addSetting(std::make_shared<IntSetting>("input_size", "Number of input neurons", 24, &conf_cache::INPUTSIZE));
    neuralSection->addSetting(std::make_shared<IntSetting>("output_size", "Number of output neurons", 9, &conf_cache::OUTPUTSIZE));
    neuralSection->addSetting(std::make_shared<IntSetting>("num_eyes", "Number of eyes per agent", 4, &conf_cache::NUMEYES));
    neuralSection->addSetting(std::make_shared<IntSetting>("brain_size", "Number of neurons in brain", 200, &conf_cache::BRAINSIZE));
    neuralSection->addSetting(std::make_shared<IntSetting>("connections_per_neuron", "Number of connections per neuron", 4, &conf_cache::CONNS));
    g_config.addSection(neuralSection);
}

Config::Config(const std::string& filename) : configFile(filename) {
    // Register all settings when config is created
    registerAllSettings();
}

Config::~Config() {
}

void Config::addSection(std::shared_ptr<ConfigSection> section) {
    sections[section->getName()] = section;
}

std::shared_ptr<ConfigSection> Config::getSection(const std::string& name) {
    auto it = sections.find(name);
    return (it != sections.end()) ? it->second : nullptr;
}

bool Config::load() {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file '" << configFile << "'. Using default values." << std::endl;
        return false;
    }
    
    std::string currentSection = "";
    std::string line;
    while (std::getline(file, line)) {
        std::string trimmed = trim(line);
        
        if (trimmed.empty() || isComment(trimmed)) {
            continue;
        }
        
        if (isSection(trimmed)) {
            currentSection = extractSectionName(trimmed);
            continue;
        }
        
        // Parse key-value pairs
        size_t equalPos = trimmed.find('=');
        if (equalPos == std::string::npos) {
            continue;
        }
        
        std::string key = trim(trimmed.substr(0, equalPos));
        std::string value = trim(trimmed.substr(equalPos + 1));
        
                if (!key.empty()) {
            // Try to find the setting in the appropriate section
            if (!currentSection.empty()) {
                auto section = getSection(currentSection);
                if (section) {
                    // For now, we'll use the legacy system to maintain compatibility
                    // In a full implementation, we'd update the section's settings directly
                }
            }
            
            // Legacy compatibility - store in the old format
            std::string fullKey = currentSection.empty() ? key : currentSection + "." + key;
            legacyValues[fullKey] = value;
        }
    }
    
    file.close();
    return true;
}

bool Config::save() {
    std::ofstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open config file '" << configFile << "' for writing." << std::endl;
        return false;
    }
    
    // Write header
    file << "# ScriptBots Configuration File" << std::endl;
    file << "# Generated automatically - do not edit manually" << std::endl;
    file << std::endl;
    
    // Write each section
    for (const auto& sectionPair : sections) {
        const auto& section = sectionPair.second;
        
        file << "# =============================================================================" << std::endl;
        file << "# " << section->getName().substr(0, 1) << section->getName().substr(1) << " Settings" << std::endl;
        file << "# =============================================================================" << std::endl;
        if (!section->getDescription().empty()) {
            file << "# " << section->getDescription() << std::endl;
        }
        file << std::endl;
        
        for (const auto& settingPair : section->getSettings()) {
            const auto& setting = settingPair.second;
            file << "# " << setting->getDescription() << std::endl;
            file << setting->getKey() << " = " << setting->saveToString() << std::endl;
            file << std::endl;
        }
    }
    
    file.close();
    return true;
}

void Config::writeToSaveFile(std::ofstream& file) {
    // Write all cached values to save file
    file.write(reinterpret_cast<const char*>(&conf_cache::WIDTH), sizeof(conf_cache::WIDTH));
    file.write(reinterpret_cast<const char*>(&conf_cache::HEIGHT), sizeof(conf_cache::HEIGHT));
    file.write(reinterpret_cast<const char*>(&conf_cache::WWIDTH), sizeof(conf_cache::WWIDTH));
    file.write(reinterpret_cast<const char*>(&conf_cache::WHEIGHT), sizeof(conf_cache::WHEIGHT));
    file.write(reinterpret_cast<const char*>(&conf_cache::CZ), sizeof(conf_cache::CZ));
    
    file.write(reinterpret_cast<const char*>(&conf_cache::NUMBOTS), sizeof(conf_cache::NUMBOTS));
    file.write(reinterpret_cast<const char*>(&conf_cache::BOTRADIUS), sizeof(conf_cache::BOTRADIUS));
    file.write(reinterpret_cast<const char*>(&conf_cache::BOTSPEED), sizeof(conf_cache::BOTSPEED));
    file.write(reinterpret_cast<const char*>(&conf_cache::SPIKESPEED), sizeof(conf_cache::SPIKESPEED));
    file.write(reinterpret_cast<const char*>(&conf_cache::SPIKEMULT), sizeof(conf_cache::SPIKEMULT));
    file.write(reinterpret_cast<const char*>(&conf_cache::BABIES), sizeof(conf_cache::BABIES));
    file.write(reinterpret_cast<const char*>(&conf_cache::BOOSTSIZEMULT), sizeof(conf_cache::BOOSTSIZEMULT));
    file.write(reinterpret_cast<const char*>(&conf_cache::REPRATEH), sizeof(conf_cache::REPRATEH));
    file.write(reinterpret_cast<const char*>(&conf_cache::REPRATEC), sizeof(conf_cache::REPRATEC));
    
    file.write(reinterpret_cast<const char*>(&conf_cache::DIST), sizeof(conf_cache::DIST));
    file.write(reinterpret_cast<const char*>(&conf_cache::METAMUTRATE1), sizeof(conf_cache::METAMUTRATE1));
    file.write(reinterpret_cast<const char*>(&conf_cache::METAMUTRATE2), sizeof(conf_cache::METAMUTRATE2));
    
    file.write(reinterpret_cast<const char*>(&conf_cache::FOODINTAKE), sizeof(conf_cache::FOODINTAKE));
    file.write(reinterpret_cast<const char*>(&conf_cache::FOODWASTE), sizeof(conf_cache::FOODWASTE));
    file.write(reinterpret_cast<const char*>(&conf_cache::FOODMAX), sizeof(conf_cache::FOODMAX));
    file.write(reinterpret_cast<const char*>(&conf_cache::FOODADDFREQ), sizeof(conf_cache::FOODADDFREQ));
    file.write(reinterpret_cast<const char*>(&conf_cache::FOODTRANSFER), sizeof(conf_cache::FOODTRANSFER));
    file.write(reinterpret_cast<const char*>(&conf_cache::FOOD_SHARING_DISTANCE), sizeof(conf_cache::FOOD_SHARING_DISTANCE));
    file.write(reinterpret_cast<const char*>(&conf_cache::FOOD_DISTRIBUTION_RADIUS), sizeof(conf_cache::FOOD_DISTRIBUTION_RADIUS));
    file.write(reinterpret_cast<const char*>(&conf_cache::REPMULT), sizeof(conf_cache::REPMULT));
    file.write(reinterpret_cast<const char*>(&conf_cache::PROP_INIT_FOOD_FILLED), sizeof(conf_cache::PROP_INIT_FOOD_FILLED));
    
    file.write(reinterpret_cast<const char*>(&conf_cache::AUTOSAVE_FREQUENCY), sizeof(conf_cache::AUTOSAVE_FREQUENCY));
    file.write(reinterpret_cast<const char*>(&conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL), sizeof(conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL));
    file.write(reinterpret_cast<const char*>(&conf_cache::RANDOM_SPAWN_COUNT), sizeof(conf_cache::RANDOM_SPAWN_COUNT));
    file.write(reinterpret_cast<const char*>(&conf_cache::INITIAL_CLOSED_ENVIRONMENT), sizeof(conf_cache::INITIAL_CLOSED_ENVIRONMENT));
    file.write(reinterpret_cast<const char*>(&conf_cache::HERBIVORE_EXTINCTION_REPOPULATION_COUNT), sizeof(conf_cache::HERBIVORE_EXTINCTION_REPOPULATION_COUNT));
    file.write(reinterpret_cast<const char*>(&conf_cache::CARNIVORE_EXTINCTION_REPOPULATION_COUNT), sizeof(conf_cache::CARNIVORE_EXTINCTION_REPOPULATION_COUNT));
    
    file.write(reinterpret_cast<const char*>(&conf_cache::INPUTSIZE), sizeof(conf_cache::INPUTSIZE));
    file.write(reinterpret_cast<const char*>(&conf_cache::OUTPUTSIZE), sizeof(conf_cache::OUTPUTSIZE));
    file.write(reinterpret_cast<const char*>(&conf_cache::NUMEYES), sizeof(conf_cache::NUMEYES));
    file.write(reinterpret_cast<const char*>(&conf_cache::BRAINSIZE), sizeof(conf_cache::BRAINSIZE));
    file.write(reinterpret_cast<const char*>(&conf_cache::CONNS), sizeof(conf_cache::CONNS));
}

void Config::readFromSaveFile(std::ifstream& file) {
    // Read all cached values from save file
    file.read(reinterpret_cast<char*>(&conf_cache::WIDTH), sizeof(conf_cache::WIDTH));
    file.read(reinterpret_cast<char*>(&conf_cache::HEIGHT), sizeof(conf_cache::HEIGHT));
    file.read(reinterpret_cast<char*>(&conf_cache::WWIDTH), sizeof(conf_cache::WWIDTH));
    file.read(reinterpret_cast<char*>(&conf_cache::WHEIGHT), sizeof(conf_cache::WHEIGHT));
    file.read(reinterpret_cast<char*>(&conf_cache::CZ), sizeof(conf_cache::CZ));
    
    file.read(reinterpret_cast<char*>(&conf_cache::NUMBOTS), sizeof(conf_cache::NUMBOTS));
    file.read(reinterpret_cast<char*>(&conf_cache::BOTRADIUS), sizeof(conf_cache::BOTRADIUS));
    file.read(reinterpret_cast<char*>(&conf_cache::BOTSPEED), sizeof(conf_cache::BOTSPEED));
    file.read(reinterpret_cast<char*>(&conf_cache::SPIKESPEED), sizeof(conf_cache::SPIKESPEED));
    file.read(reinterpret_cast<char*>(&conf_cache::SPIKEMULT), sizeof(conf_cache::SPIKEMULT));
    file.read(reinterpret_cast<char*>(&conf_cache::BABIES), sizeof(conf_cache::BABIES));
    file.read(reinterpret_cast<char*>(&conf_cache::BOOSTSIZEMULT), sizeof(conf_cache::BOOSTSIZEMULT));
    file.read(reinterpret_cast<char*>(&conf_cache::REPRATEH), sizeof(conf_cache::REPRATEH));
    file.read(reinterpret_cast<char*>(&conf_cache::REPRATEC), sizeof(conf_cache::REPRATEC));
    
    file.read(reinterpret_cast<char*>(&conf_cache::DIST), sizeof(conf_cache::DIST));
    file.read(reinterpret_cast<char*>(&conf_cache::METAMUTRATE1), sizeof(conf_cache::METAMUTRATE1));
    file.read(reinterpret_cast<char*>(&conf_cache::METAMUTRATE2), sizeof(conf_cache::METAMUTRATE2));
    
    file.read(reinterpret_cast<char*>(&conf_cache::FOODINTAKE), sizeof(conf_cache::FOODINTAKE));
    file.read(reinterpret_cast<char*>(&conf_cache::FOODWASTE), sizeof(conf_cache::FOODWASTE));
    file.read(reinterpret_cast<char*>(&conf_cache::FOODMAX), sizeof(conf_cache::FOODMAX));
    file.read(reinterpret_cast<char*>(&conf_cache::FOODADDFREQ), sizeof(conf_cache::FOODADDFREQ));
    file.read(reinterpret_cast<char*>(&conf_cache::FOODTRANSFER), sizeof(conf_cache::FOODTRANSFER));
    file.read(reinterpret_cast<char*>(&conf_cache::FOOD_SHARING_DISTANCE), sizeof(conf_cache::FOOD_SHARING_DISTANCE));
    file.read(reinterpret_cast<char*>(&conf_cache::FOOD_DISTRIBUTION_RADIUS), sizeof(conf_cache::FOOD_DISTRIBUTION_RADIUS));
    file.read(reinterpret_cast<char*>(&conf_cache::REPMULT), sizeof(conf_cache::REPMULT));
    file.read(reinterpret_cast<char*>(&conf_cache::PROP_INIT_FOOD_FILLED), sizeof(conf_cache::PROP_INIT_FOOD_FILLED));
    
    file.read(reinterpret_cast<char*>(&conf_cache::AUTOSAVE_FREQUENCY), sizeof(conf_cache::AUTOSAVE_FREQUENCY));
    file.read(reinterpret_cast<char*>(&conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL), sizeof(conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL));
    file.read(reinterpret_cast<char*>(&conf_cache::RANDOM_SPAWN_COUNT), sizeof(conf_cache::RANDOM_SPAWN_COUNT));
    file.read(reinterpret_cast<char*>(&conf_cache::INITIAL_CLOSED_ENVIRONMENT), sizeof(conf_cache::INITIAL_CLOSED_ENVIRONMENT));
    file.read(reinterpret_cast<char*>(&conf_cache::HERBIVORE_EXTINCTION_REPOPULATION_COUNT), sizeof(conf_cache::HERBIVORE_EXTINCTION_REPOPULATION_COUNT));
    file.read(reinterpret_cast<char*>(&conf_cache::CARNIVORE_EXTINCTION_REPOPULATION_COUNT), sizeof(conf_cache::CARNIVORE_EXTINCTION_REPOPULATION_COUNT));
    
    file.read(reinterpret_cast<char*>(&conf_cache::INPUTSIZE), sizeof(conf_cache::INPUTSIZE));
    file.read(reinterpret_cast<char*>(&conf_cache::OUTPUTSIZE), sizeof(conf_cache::OUTPUTSIZE));
    file.read(reinterpret_cast<char*>(&conf_cache::NUMEYES), sizeof(conf_cache::NUMEYES));
    file.read(reinterpret_cast<char*>(&conf_cache::BRAINSIZE), sizeof(conf_cache::BRAINSIZE));
    file.read(reinterpret_cast<char*>(&conf_cache::CONNS), sizeof(conf_cache::CONNS));
}

void Config::generateConfigTemplate(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open template file '" << filename << "' for writing." << std::endl;
        return;
    }
    
    file << "# ScriptBots Configuration Template" << std::endl;
    file << "# This file shows all available settings with their descriptions and default values" << std::endl;
    file << "# Copy this file to scriptbots.conf and modify as needed" << std::endl;
    file << std::endl;
    
    for (const auto& sectionPair : sections) {
        const auto& section = sectionPair.second;
        
        file << "# =============================================================================" << std::endl;
        file << "# " << section->getName().substr(0, 1) << section->getName().substr(1) << " Settings" << std::endl;
        file << "# =============================================================================" << std::endl;
        if (!section->getDescription().empty()) {
            file << "# " << section->getDescription() << std::endl;
        }
        file << std::endl;
        
        for (const auto& settingPair : section->getSettings()) {
            const auto& setting = settingPair.second;
            file << "# " << setting->getDescription() << std::endl;
            file << "# Default: " << setting->saveToString() << std::endl;
            file << "# " << setting->getKey() << " = " << setting->saveToString() << std::endl;
            file << std::endl;
        }
    }
    
    file.close();
}

// Legacy compatibility methods
int Config::getInt(const std::string& key, int defaultValue) const {
    auto it = legacyValues.find(key);
    if (it == legacyValues.end()) {
        return defaultValue;
    }
    
    try {
        return std::stoi(it->second);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

float Config::getFloat(const std::string& key, float defaultValue) const {
    auto it = legacyValues.find(key);
    if (it == legacyValues.end()) {
        return defaultValue;
    }
    
    try {
        return std::stof(it->second);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

bool Config::getBool(const std::string& key, bool defaultValue) const {
    auto it = legacyValues.find(key);
    if (it == legacyValues.end()) {
        return defaultValue;
    }
    
    std::string value = it->second;
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    
    return value == "true" || value == "1" || value == "yes" || value == "on";
}

std::string Config::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = legacyValues.find(key);
    if (it == legacyValues.end()) {
        return defaultValue;
    }
    return it->second;
}

void Config::setInt(const std::string& key, int value) {
    legacyValues[key] = std::to_string(value);
}

void Config::setFloat(const std::string& key, float value) {
    legacyValues[key] = std::to_string(value);
}

void Config::setBool(const std::string& key, bool value) {
    legacyValues[key] = value ? "true" : "false";
}

void Config::setString(const std::string& key, const std::string& value) {
    legacyValues[key] = value;
}

bool Config::hasKey(const std::string& key) const {
    return legacyValues.find(key) != legacyValues.end();
}

std::vector<std::string> Config::getKeys() const {
    std::vector<std::string> keys;
    for (const auto& pair : legacyValues) {
        keys.push_back(pair.first);
    }
    return keys;
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

std::string Config::extractSectionName(const std::string& line) {
    if (line.length() < 3) return "";
    return line.substr(1, line.length() - 2);
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