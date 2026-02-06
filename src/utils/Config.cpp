#include "Config.h"
#include <stdexcept>

void Config::set(const std::string &key, ConfigValue value) {
    getInstance().settings[key] = value;
}

void Config::load() {
    getInstance().loadDefaults();
}

void Config::loadDefaults() {
    settings.clear();
    // Temp settings
    settings["map_radius"] = 50;
    settings["log_file_name"] = "hexLife.log";

    //Noise Map Settings
    settings["seed_temp"] = 11387; // Random big prime
    settings["seed_moist"] = 588487; // Random big prime

    settings["noise_octave"] = 4;
    settings["noise_persistence"] = 0.5f;
    settings["noise_lacunarity"] = 2.0f;

    settings["scale_temp"] = 0.022f; // Small -> low change
    settings["scale_moist"] = 0.0275f; // Big -> high change
    settings["stretch_min"] = 0.15f;
    settings["stretch_max"] = 0.85f;

    // Biome Selection
    settings["threshold_max_polar_temp"] = 0.3f;
    settings["threshold_max_warm_temp"] = 0.64f;
    settings["threshold_max_plain_moist"] = 0.55f;
    settings["threshold_max_desert_moist"] = 0.46f;

    // Map Visualisation
    settings["background_color"] = 10;
    settings["hex_border_darkness"] = 20;

    // Color settins
    settings["Polar_Color_r"] = 230;
    settings["Polar_Color_g"] = 245;
    settings["Polar_Color_b"] = 255;

    settings["Plains_Color_r"] = 170;
    settings["Plains_Color_g"] = 200;
    settings["Plains_Color_b"] = 100;

    settings["Forest_Color_r"] = 34;
    settings["Forest_Color_g"] = 139;
    settings["Forest_Color_b"] = 34;

    settings["Desert_Color_r"] = 240;
    settings["Desert_Color_g"] = 210;
    settings["Desert_Color_b"] = 130;

    settings["Jungle_Color_r"] = 20;
    settings["Jungle_Color_g"] = 100;
    settings["Jungle_Color_b"] = 80;

    settings["Default_Color"] = 20;
}
