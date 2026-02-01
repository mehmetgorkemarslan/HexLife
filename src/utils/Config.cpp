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
    settings["map_radius"] = 10;
    settings["log_file_name"] = "hexLife.log";
}
