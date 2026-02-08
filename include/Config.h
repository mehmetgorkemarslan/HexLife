#pragma once
#include <string>
#include "json.h"
#include "Logger.h"

using json = nlohmann::json;

/**
 * @brief Application configuration manager (singleton).
 * @details Provides access to configuration data loaded from a JSON file.
 */
class Config {
public:
    // Singleton
    /**
     * @brief Get the singleton instance of Config.
     * @return Reference to the Config singleton.
     */
    static Config &getInstance() {
        static Config instance;
        return instance;
    }

    // Prevent duplication
    Config(const Config &) = delete;

    void operator=(const Config &) = delete;

    /**
     * @brief Load configuration from a JSON file path.
     * @param path Filesystem path to the JSON config file. Defaults to "config.json".
     * @details Implementation opens the file, parses JSON into internal storage and logs errors on failure.
     */
    static void load(const std::string &path = "config.json");

    // int radius = Config::get<int>("map", "radius);
    /**
     * @brief Retrieve a typed value from the configuration.
     * @param section Top-level JSON section name.
     * @param key Key inside the section.
     * @return The requested value as type T. Returns default-constructed T on error.
     * @details On any lookup or conversion error this method logs an error and returns T().
     */
    template<typename T>
    static T get(const std::string &section, const std::string &key) {
        try {
            return getInstance()._data.at(section).at(key).get<T>();
        } catch (...) {
            Logger::Log(LogLevel::ERROR,
                        std::format("[Config] {}.{} can not find. Returning default value", section, key));
            return T();
        }
    }

    // auto mapSettings = Config::getSection("map");
    // int radius = mapSettings.at("radius").get<int>();
    /**
     * @brief Retrieve a JSON subsection by name.
     * @param section Top-level JSON section name.
     * @return A json object representing the section, or empty json on error.
     */
    static json getSection(const std::string &section) {
        try {
            return getInstance()._data.at(section);
        } catch (...) {
            Logger::Log(LogLevel::ERROR,
                        std::format("[Config] section {} can not find. Returning empty json", section));
            return {};
        }
    }

    // auto colors = Config::getInnerSection("visuals", "colors");
    // int ColorRed = colors.at("polar").at(0).get<int>();
    /**
     * @brief Retrieve an inner JSON subsection from a top-level section.
     * @param outSec Top-level section name.
     * @param inSec Inner section name within outSec.
     * @return A json object representing the inner section, or empty json on error.
     */
    static json getInnerSection(const std::string &outSec, const std::string &inSec) {
        try {
            return getInstance()._data.at(outSec).at(inSec);
        } catch (...) {
            Logger::Log(LogLevel::ERROR,
                        std::format("[Config] section {}.{} can not find. Returning empty json", outSec, inSec));
            return {};
        }
    }

private:
    Config() = default;

    json _data;
};
