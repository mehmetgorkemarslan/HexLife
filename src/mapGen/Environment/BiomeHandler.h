#pragma once
#include <unordered_map>
#include "Common.h"
#include "Biome.h"
#include "Config.h"

namespace mapGen {

    /**
     * @brief Manages available biomes and provides lookup utilities.
     * @details Stores a vector of Biome instances and a name->id map populated by loadBiomes().
     */
    class BiomeHandler {
    public:
        static BiomeHandler &getInstance() {
            static BiomeHandler instance;
            return instance;
        }

        /**
         * @brief Load biome definitions from configuration and populate internal registry.
         * @details Clears existing entries, reads the "biome.types" section from Config,
         *          constructs Biome objects assigning sequential IDs starting from 0,
         *          and fills the name->id map. Must be called at application startup
         *          before performing biome lookups.
         */
        static void loadBiomes();

        /**
         * @brief Retrieve a biome by its numeric id.
         * @param id BiomeID index into the internal biome vector.
         * @return Reference to the Biome instance.
         * @note No bounds check is performed here; caller must ensure id is valid.
         */
        static Biome &getBiomeById(BiomeID id){
            return getInstance().biomes[id];
        };

        /**
         * @brief Retrieve a BiomeID by biome name.
         * @param name The biome name as configured.
         * @return The BiomeID associated with the name.
         * @* @throws std::out_of_range If the biome name is not registered in the handler.
         */
        static BiomeID getBiomeIdByName(const std::string &name){
            return getInstance().nameToId.at(name);
        }

        /**
         * @brief Determine biome id based on temperature and moisture thresholds.
         * @param temperature Temperature value used to categorize cold/warm ranges.
         * @param moisture Moisture value used to distinguish plains/desert/jungle.
         * @return BiomeID selected according to configured threshold keys
         * @details The function caches commonly used biome ids and threshold values for performance.
         */
        static BiomeID calculateBiomeID(float temperature, float moisture);

    private:
        BiomeHandler() = default;

        std::vector<Biome> biomes;

        std::unordered_map<std::string, BiomeID> nameToId;

        /**
         * @brief Parse color array from a JSON section.
         * @param section JSON section that contains a "color" array of three integers [r,g,b].
         * @return Color constructed from the three uint8_t values.
         */
        static Color getColor(const json &section) {
            auto cSection = section.at("color");
            return {
                cSection.at(0).get<uint8_t>(), cSection.at(1).get<uint8_t>(),
                cSection.at(2).get<uint8_t>()
            };
        }

        /**
         * @brief Parse biome stats from a JSON section.
         * @param section JSON section that contains a "stats" object.
         * @return BiomeStats with movement_cost and fertility (defaults applied if missing).
         */
        static BiomeStats getBiomeStats(const json &section) {
            auto statSection = section.at("stats");

            const float move = statSection.value("movement_cost", 1.0f);
            const float fert = statSection.value("fertility", 0.5f);
            return {move, fert};
        };
    };
}
