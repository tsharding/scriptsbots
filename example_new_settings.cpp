// Example: Adding new settings to ScriptBots
// This demonstrates how the modular configuration system makes it easy to add new settings

#include "Config.h"

// Example 1: Adding a new setting to an existing section
void addNewAgentSetting() {
    // Get the agent section
    auto agentSection = g_config.getSection("agent");
    if (agentSection) {
        // Add a new setting for agent memory capacity
        agentSection->addSetting(std::make_shared<IntSetting>(
            "memory_capacity", 
            "Number of memories each agent can store", 
            10,  // default value
            nullptr  // no cache pointer needed for this example
        ));
    }
}

// Example 2: Creating a completely new section
void addNewGraphicsSection() {
    // Create a new graphics section
    auto graphicsSection = std::make_shared<ConfigSection>("graphics", "Graphics and rendering settings");
    
    // Add various graphics settings
    graphicsSection->addSetting(std::make_shared<BoolSetting>(
        "vsync", 
        "Enable vertical synchronization", 
        true
    ));
    
    graphicsSection->addSetting(std::make_shared<IntSetting>(
        "max_fps", 
        "Maximum frames per second (0 = unlimited)", 
        60
    ));
    
    graphicsSection->addSetting(std::make_shared<FloatSetting>(
        "particle_alpha", 
        "Transparency of particle effects (0.0 to 1.0)", 
        0.7f
    ));
    
    graphicsSection->addSetting(std::make_shared<StringSetting>(
        "render_mode", 
        "Rendering mode: 'fast', 'quality', or 'ultra'", 
        "quality"
    ));
    
    // Add the section to the global config
    g_config.addSection(graphicsSection);
}

// Example 3: Adding settings with validation
class ValidatedIntSetting : public IntSetting {
private:
    int minValue;
    int maxValue;
    
public:
    ValidatedIntSetting(const std::string& k, const std::string& desc, 
                       int defVal, int min, int max, int* cache = nullptr)
        : IntSetting(k, desc, defVal, cache), minValue(min), maxValue(max) {}
    
protected:
    int parseValue(const std::string& str) override {
        int value = IntSetting::parseValue(str);
        if (value < minValue) {
            std::cerr << "Warning: " << getKey() << " value " << value 
                      << " is below minimum " << minValue << ". Using minimum." << std::endl;
            return minValue;
        }
        if (value > maxValue) {
            std::cerr << "Warning: " << getKey() << " value " << value 
                      << " is above maximum " << maxValue << ". Using maximum." << std::endl;
            return maxValue;
        }
        return value;
    }
};

void addValidatedSettings() {
    auto simulationSection = g_config.getSection("simulation");
    if (simulationSection) {
        // Add a setting with validation (0-1000 range)
        simulationSection->addSetting(std::make_shared<ValidatedIntSetting>(
            "max_agents", 
            "Maximum number of agents allowed (0-1000)", 
            500,  // default
            0,    // min
            1000, // max
            nullptr
        ));
    }
}

// Example 4: Adding a setting that affects multiple cached values
void addComplexSetting() {
    auto worldSection = g_config.getSection("world");
    if (worldSection) {
        // Add a setting that automatically updates related cached values
        auto cellSizeSetting = std::make_shared<IntSetting>(
            "cell_size", 
            "Cell size in pixels for food squares", 
            50, 
            &conf_cache::CZ
        );
        
        // You could add a callback to update related values
        // For example, when cell size changes, you might want to update
        // the number of cells in the world
    }
}

// Example 5: Adding a setting with custom serialization
class ColorSetting : public StringSetting {
public:
    ColorSetting(const std::string& k, const std::string& desc, 
                const std::string& defVal, std::string* cache = nullptr)
        : StringSetting(k, desc, defVal, cache) {}
    
    // Custom validation for color values
    void loadFromString(const std::string& str) override {
        // Validate color format (e.g., "#RRGGBB" or "red", "green", etc.)
        if (isValidColor(str)) {
            StringSetting::loadFromString(str);
        } else {
            std::cerr << "Warning: Invalid color value '" << str << "' for " << getKey() 
                      << ". Using default." << std::endl;
            StringSetting::loadFromString(getDefaultValue());
        }
    }
    
private:
    bool isValidColor(const std::string& color) {
        // Simple validation - could be more sophisticated
        if (color.empty()) return false;
        if (color[0] == '#') {
            // Hex color format
            return color.length() == 7 && 
                   color.substr(1).find_first_not_of("0123456789ABCDEFabcdef") == std::string::npos;
        }
        // Named colors (simplified)
        std::vector<std::string> validColors = {"red", "green", "blue", "yellow", "cyan", "magenta", "white", "black"};
        return std::find(validColors.begin(), validColors.end(), color) != validColors.end();
    }
    
    std::string getDefaultValue() const {
        return "#FFFFFF"; // white
    }
};

void addColorSettings() {
    auto graphicsSection = g_config.getSection("graphics");
    if (graphicsSection) {
        graphicsSection->addSetting(std::make_shared<ColorSetting>(
            "background_color", 
            "Background color (hex format or named color)", 
            "#FFFFFF"
        ));
        
        graphicsSection->addSetting(std::make_shared<ColorSetting>(
            "agent_color", 
            "Default agent color", 
            "#0000FF"
        ));
    }
}

// Example 6: Adding a setting with dependencies
class DependentSetting : public FloatSetting {
private:
    std::string dependsOn;
    float minRatio;
    
public:
    DependentSetting(const std::string& k, const std::string& desc, 
                    float defVal, const std::string& dependency, float ratio, float* cache = nullptr)
        : FloatSetting(k, desc, defVal, cache), dependsOn(dependency), minRatio(ratio) {}
    
    void loadFromString(const std::string& str) override {
        float value = FloatSetting::parseValue(str);
        
        // Check dependency
        auto worldSection = g_config.getSection("world");
        if (worldSection) {
            float dependencyValue = worldSection->get<float>(dependsOn, 0.0f);
            if (value < dependencyValue * minRatio) {
                std::cerr << "Warning: " << getKey() << " value " << value 
                          << " is too small relative to " << dependsOn << " (" << dependencyValue 
                          << "). Minimum ratio is " << minRatio << std::endl;
                value = dependencyValue * minRatio;
            }
        }
        
        set(value);
    }
};

void addDependentSettings() {
    auto worldSection = g_config.getSection("world");
    if (worldSection) {
        // Add a setting that depends on world size
        worldSection->addSetting(std::make_shared<DependentSetting>(
            "minimum_view_distance", 
            "Minimum view distance (must be at least 1% of world width)", 
            100.0f,
            "width",  // depends on world width
            0.01f,    // minimum 1% of width
            nullptr
        ));
    }
}

// Main function to demonstrate all examples
void demonstrateNewSettings() {
    std::cout << "Adding new settings to ScriptBots configuration..." << std::endl;
    
    // Add various types of new settings
    addNewAgentSetting();
    addNewGraphicsSection();
    addValidatedSettings();
    addComplexSetting();
    addColorSettings();
    addDependentSettings();
    
    // Generate a template showing all available settings
    g_config.generateConfigTemplate("scriptbots_template.conf");
    
    std::cout << "New settings added successfully!" << std::endl;
    std::cout << "Generated template file: scriptbots_template.conf" << std::endl;
}

/*
Benefits of this modular approach:

1. **Single Point of Registration**: All settings are registered in one place (registerAllSettings())
2. **Type Safety**: Each setting has a specific type with validation
3. **Documentation**: Settings are self-documenting with descriptions
4. **Validation**: Easy to add custom validation logic
5. **Dependencies**: Settings can depend on other settings
6. **Extensibility**: Easy to add new setting types
7. **Backward Compatibility**: Legacy code continues to work
8. **Performance**: Cached values for fast access
9. **Serialization**: Automatic save/load support
10. **Template Generation**: Automatic documentation generation

To add a new setting, you only need to:
1. Add it to the appropriate section in registerAllSettings()
2. Optionally add a cache variable if performance is critical
3. Optionally add an accessor function in the conf:: namespace

This is much easier than the old approach which required changes in 4+ places!
*/