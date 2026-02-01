#include "Cell.h"

namespace mapGen {
    void Cell::addEntity(Entity *entity) {
        this->entities.push_back(entity);
    }

    void Cell::removeEntity(Entity *entity) {
        this->entities.erase(std::ranges::find(entities, entity));
    }

    void Cell::getRepresentation(std::stringstream &ss) {
        // TODO: change with biome, is there any enitiy
        ss << "[C] " ;
    }
}
