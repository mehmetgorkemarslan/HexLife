#include "Cell.h"

namespace mapGen {
    void Cell::addEntity(eco::Entity *entity) {
        this->entities.push_back(entity);
    }

    void Cell::removeEntity(eco::Entity *entity) {
        auto it = std::ranges::find(entities, entity);
        if (it != entities.end()) {
            this->entities.erase(it);
        }
    }

    int Cell::getEntityCount() const {
        return entities.size();
    }

    void Cell::setBiome(BiomeID biome) {
        this->biome = biome;
    }

    BiomeID Cell::getBiomeID() const {
        return this->biome;
    }

    void Cell::getRepresentation(std::stringstream &ss) {
        ss << "[" << (int) (temperature * 100) << "-" << (int) (moisture * 100) << "] ";
    }
}
