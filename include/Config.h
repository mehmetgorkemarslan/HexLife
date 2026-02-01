#pragma once
#include <variant>
#include <string>
#include <unordered_map>
#include <stdexcept>


using ConfigValue = std::variant<int, float, bool, std::string>;

class Config {
public:
    static Config &getInstance() {
        static Config instance;
        return instance;
    }

    // Prevent duplication
    Config(const Config &) = delete;

    void operator=(const Config &) = delete;


    static void set(const std::string &key, ConfigValue value);

    template<typename T>
    static T get(const std::string &key){
        auto &s = getInstance().settings;
        auto it = s.find(key);
        if (it == s.end()) {
            throw std::runtime_error("Config key not found: " + key);
        }
        return std::get<T>(it->second);
    }


    static void load();

private:
    void loadDefaults();

    Config() {
    }

    std::unordered_map<std::string, ConfigValue> settings;
};
