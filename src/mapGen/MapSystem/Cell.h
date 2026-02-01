#pragma once
#include <cstdint>
#include <vector>
#include <sstream>

namespace mapGen {
    /**
     * @brief Forward declaration for Entity.
     *
     * Represents a small actor or object that can occupy a Cell.
     * Full definition is provided elsewhere.
     */
    class Entity;

    /**
     * @brief Temporary biome classification for cells.
     *
     * These values provide a simple terrain categorization used for
     * visualization and simple behavior decisions during generation.
     */
    enum class BiomeType: uint8_t {
        OCEAN,
        DESERT,
        FOREST,
        VALLEY,
        MOUNTAIN
    };

    /**
     * @brief Represents a single hex grid cell and its contents.
     *
     * Holds a biome type, a list of entities present in the cell, and
     * helper methods to manage entities and produce a textual representation
     * for logging or debugging.
     */
    struct Cell {
        BiomeType biome;

        // Other variables going to here

        std::vector<Entity *> entities;

        /**
         * @brief Add an entity pointer to this cell.
         *
         * The cell takes ownership of the pointer lifecycle is not implied;
         * the method simply stores the pointer in the internal list.
         *
         * @param entity Pointer to the Entity to add.
         */
        void addEntity(Entity *entity);

        /**
         * @brief Remove the specified entity pointer from this cell.
         *
         * If the entity is not found, behavior depends on the underlying container
         * erase implementation; callers should ensure the entity exists before removal.
         *
         * @param entity Pointer to the Entity to remove.
         */
        void removeEntity(Entity *entity);

        /**
         * @brief Append a short textual representation of the cell to the given stringstream.
         *
         * Used for logging or simple ASCII visualization. Implementations typically
         * include biome information and a marker if entities are present.
         *
         * @param ss Destination stringstream where the representation is appended.
         */
        void getRepresentation(std::stringstream &ss);
    };
}
