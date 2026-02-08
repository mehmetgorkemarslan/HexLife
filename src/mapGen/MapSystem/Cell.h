#pragma once
#include <vector>
#include <sstream>
#include "Common.h"

namespace mapGen {
    /**
     * @brief Forward declaration for Entity.
     * @details Represents an actor or object that can occupy a Cell; full definition is elsewhere.
     */
    class Entity;


    /**
     * @brief Represents a single hex grid cell and its contents.
     * @details Stores biome, environmental properties and a list of entity pointers.
     *          Methods manage entities and provide simple textual representation.
     */
    struct Cell {
        /** @brief Primary biome classification for this cell. */
        BiomeID biome;

        std::vector<Entity *> entities;

        float temperature = 0.0f;
        float moisture = 0.0f;

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
         * @brief Return number of entities currently stored in this cell.
         * @returns Count of entities.
         */
        int getEntityCount() const;

        /**
         * @brief Set the biome type for this cell.
         * @param biome New biome Id to assign.
         */
        void setBiome(BiomeID biome);

        /**
         * @brief Get the biome type assigned to this cell.
         * @returns Current BiomeID for the cell.
         */
        BiomeID getBiomeID() const;

        /**
         * @brief Append a short textual representation of the cell to the given stringstream.
         * @param ss Destination stringstream where the representation is appended.
         */
        void getRepresentation(std::stringstream &ss);
    };
}
