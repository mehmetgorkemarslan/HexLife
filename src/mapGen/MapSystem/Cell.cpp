#include "Cell.h"

namespace mapGen {
    void Cell::addEntity(Entity *entity) {
        this->entities.push_back(entity);
    }

    void Cell::removeEntity(Entity *entity) {
        this->entities.erase(std::ranges::find(entities, entity));
    }

    int Cell::getEntityCount() const {
        return entities.size();
    }

    void Cell::setBiome(BiomeType biome) {
        this->biome = biome;
    }

    BiomeType Cell::getBiome() const {
        return this->biome;
    }

    void Cell::getRepresentation(std::stringstream &ss) {
        ss << "[" << (int) (temperature * 100) << "-" << (int) (moisture * 100) << "] ";
    }
}
