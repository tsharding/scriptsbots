#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>

// Forward declarations
class ConfigSetting;
class ConfigSection;

// Setting value types
enum class SettingType {
    INT,
    FLOAT,
    BOOL,
    STRING
};

// Base class for configuration settings
class ConfigSetting {
protected:
    std::string key;
    std::string description;
    SettingType type;
    bool cached;
    
public:
    ConfigSetting(const std::string& k, const std::string& desc, SettingType t, bool cache = true)
        : key(k), description(desc), type(t), cached(cache) {}
    
    virtual ~ConfigSetting() = default;
    
    const std::string& getKey() const { return key; }
    const std::string& getDescription() const { return description; }
    SettingType getType() const { return type; }
    bool isCached() const { return cached; }
    
    virtual void loadFromString(const std::string& value) = 0;
    virtual std::string saveToString() const = 0;
    virtual void writeToSaveFile(std::ofstream& file) = 0;
    virtual void readFromSaveFile(std::ifstream& file) = 0;
    virtual void resetToDefault() = 0;
};

// Template for typed settings
template<typename T>
class TypedConfigSetting : public ConfigSetting {
protected:
    T value;
    T defaultValue;
    T* cachePtr;
    
public:
    TypedConfigSetting(const std::string& k, const std::string& desc, 
                      const T& defVal, T* cache = nullptr, bool cache_val = true)
        : ConfigSetting(k, desc, getTypeFor(), cache_val), 
          value(defVal), defaultValue(defVal), cachePtr(cache) {}
    
    T get() const { return value; }
    void set(const T& newValue) { 
        value = newValue; 
        if (cachePtr) *cachePtr = newValue;
    }
    
    void resetToDefault() override {
        value = defaultValue;
        if (cachePtr) *cachePtr = defaultValue;
    }
    
    void loadFromString(const std::string& str) override {
        value = parseValue(str);
        if (cachePtr) *cachePtr = value;
    }
    
    std::string saveToString() const override {
        return toString(value);
    }
    
    void writeToSaveFile(std::ofstream& file) override {
        file.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }
    
    void readFromSaveFile(std::ifstream& file) override {
        file.read(reinterpret_cast<char*>(&value), sizeof(T));
        if (cachePtr) *cachePtr = value;
    }
    
protected:
    virtual T parseValue(const std::string& str) = 0;
    virtual std::string toString(const T& val) const = 0;
    
    static SettingType getTypeFor() {
        if (std::is_same<T, int>::value) return SettingType::INT;
        else if (std::is_same<T, float>::value) return SettingType::FLOAT;
        else if (std::is_same<T, bool>::value) return SettingType::BOOL;
        else if (std::is_same<T, std::string>::value) return SettingType::STRING;
        else {
            // This will cause a compile error for unsupported types
            static_assert(std::is_same<T, int>::value || 
                         std::is_same<T, float>::value || 
                         std::is_same<T, bool>::value || 
                         std::is_same<T, std::string>::value, 
                         "Unsupported type");
            return SettingType::INT; // This should never be reached
        }
    }
};

// Specialized setting classes
class IntSetting : public TypedConfigSetting<int> {
public:
    IntSetting(const std::string& k, const std::string& desc, int defVal, int* cache = nullptr)
        : TypedConfigSetting(k, desc, defVal, cache) {}
    
protected:
    int parseValue(const std::string& str) override {
        try { return std::stoi(str); }
        catch (...) { return defaultValue; }
    }
    
    std::string toString(const int& val) const override {
        return std::to_string(val);
    }
};

class FloatSetting : public TypedConfigSetting<float> {
public:
    FloatSetting(const std::string& k, const std::string& desc, float defVal, float* cache = nullptr)
        : TypedConfigSetting(k, desc, defVal, cache) {}
    
protected:
    float parseValue(const std::string& str) override {
        try { return std::stof(str); }
        catch (...) { return defaultValue; }
    }
    
    std::string toString(const float& val) const override {
        return std::to_string(val);
    }
};

class BoolSetting : public TypedConfigSetting<bool> {
public:
    BoolSetting(const std::string& k, const std::string& desc, bool defVal, bool* cache = nullptr)
        : TypedConfigSetting(k, desc, defVal, cache) {}
    
protected:
    bool parseValue(const std::string& str) override {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower == "true" || lower == "1" || lower == "yes" || lower == "on";
    }
    
    std::string toString(const bool& val) const override {
        return val ? "true" : "false";
    }
};

class StringSetting : public TypedConfigSetting<std::string> {
public:
    StringSetting(const std::string& k, const std::string& desc, const std::string& defVal, std::string* cache = nullptr)
        : TypedConfigSetting(k, desc, defVal, cache) {}
    
protected:
    std::string parseValue(const std::string& str) override {
        return str;
    }
    
    std::string toString(const std::string& val) const override {
        return val;
    }
};

// Configuration section
class ConfigSection {
private:
    std::string name;
    std::string description;
    std::map<std::string, std::shared_ptr<ConfigSetting>> settings;
    
public:
    ConfigSection(const std::string& n, const std::string& desc = "")
        : name(n), description(desc) {}
    
    void addSetting(std::shared_ptr<ConfigSetting> setting) {
        settings[setting->getKey()] = setting;
    }
    
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    const std::map<std::string, std::shared_ptr<ConfigSetting>>& getSettings() const { return settings; }
    
    template<typename T>
    T get(const std::string& key, const T& defaultValue) const {
        auto it = settings.find(key);
        if (it != settings.end()) {
            auto typedSetting = std::dynamic_pointer_cast<TypedConfigSetting<T>>(it->second);
            if (typedSetting) {
                return typedSetting->get();
            }
        }
        return defaultValue;
    }
    
    template<typename T>
    void set(const std::string& key, const T& value) {
        auto it = settings.find(key);
        if (it != settings.end()) {
            auto typedSetting = std::dynamic_pointer_cast<TypedConfigSetting<T>>(it->second);
            if (typedSetting) {
                typedSetting->set(value);
            }
        }
    }
};

// Main configuration class
class Config {
private:
    std::map<std::string, std::shared_ptr<ConfigSection>> sections;
    std::map<std::string, std::string> legacyValues; // For backward compatibility
    std::string configFile;
    
    void parseLine(const std::string& line);
    std::string trim(const std::string& str);
    bool isComment(const std::string& line);
    bool isSection(const std::string& line);
    std::string extractSectionName(const std::string& line);
    
public:
    Config(const std::string& filename = "scriptbots.conf");
    ~Config();
    
    // Section management
    void addSection(std::shared_ptr<ConfigSection> section);
    std::shared_ptr<ConfigSection> getSection(const std::string& name);
    
    // File operations
    bool load();
    bool save();
    
    // Legacy compatibility methods
    int getInt(const std::string& key, int defaultValue = 0) const;
    float getFloat(const std::string& key, float defaultValue = 0.0f) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    
    void setInt(const std::string& key, int value);
    void setFloat(const std::string& key, float value);
    void setBool(const std::string& key, bool value);
    void setString(const std::string& key, const std::string& value);
    
    bool hasKey(const std::string& key) const;
    std::vector<std::string> getKeys() const;
    
    // Save file compatibility
    void writeToSaveFile(std::ofstream& file);
    void readFromSaveFile(std::ifstream& file);
    
    // Documentation
    void generateConfigTemplate(const std::string& filename);
    
    // Legacy compatibility - cache values from legacy system
    void cacheValues();
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
    extern float PROP_INIT_FOOD_FILLED;
    
    // Simulation settings
    extern int AUTOSAVE_FREQUENCY;
    extern int RANDOM_SPAWN_EPOCH_INTERVAL;
    extern int RANDOM_SPAWN_COUNT;
    extern bool INITIAL_CLOSED_ENVIRONMENT;
    extern int HERBIVORE_EXTINCTION_REPOPULATION_COUNT;
    extern int CARNIVORE_EXTINCTION_REPOPULATION_COUNT;
    
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
    inline float PROP_INIT_FOOD_FILLED() { return conf_cache::PROP_INIT_FOOD_FILLED; }
    
    // Simulation settings
    inline int AUTOSAVE_FREQUENCY() { return conf_cache::AUTOSAVE_FREQUENCY; }
    inline int RANDOM_SPAWN_EPOCH_INTERVAL() { return conf_cache::RANDOM_SPAWN_EPOCH_INTERVAL; }
    inline int RANDOM_SPAWN_COUNT() { return conf_cache::RANDOM_SPAWN_COUNT; }
    inline bool INITIAL_CLOSED_ENVIRONMENT() { return conf_cache::INITIAL_CLOSED_ENVIRONMENT; }
    inline int HERBIVORE_EXTINCTION_REPOPULATION_COUNT() { return conf_cache::HERBIVORE_EXTINCTION_REPOPULATION_COUNT; }
    inline int CARNIVORE_EXTINCTION_REPOPULATION_COUNT() { return conf_cache::CARNIVORE_EXTINCTION_REPOPULATION_COUNT; }
    
    // Neural network settings
    inline int INPUTSIZE() { return conf_cache::INPUTSIZE; }
    inline int OUTPUTSIZE() { return conf_cache::OUTPUTSIZE; }
    inline int NUMEYES() { return conf_cache::NUMEYES; }
    inline int BRAINSIZE() { return conf_cache::BRAINSIZE; }
    inline int CONNS() { return conf_cache::CONNS; }
}

// Configuration registration function
void registerAllSettings();

#endif 