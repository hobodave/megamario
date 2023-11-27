#pragma once

#include "Entity.hpp"

#include <vector>
#include <unordered_map>

typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::unordered_map<std::string, EntityVector> EntityMap;

class EntityManager
{
private:
    EntityVector    m_entities;
    EntityVector    m_entitiesToAdd;
    EntityMap       m_entityMap;
    size_t          m_entityCount {0};

    void removeDeadEntities(EntityVector& entities);

public:
    // Constructor
    EntityManager() = default;

    // Destructor
    ~EntityManager();

    void update();

    std::shared_ptr<Entity> addEntity(const std::string& tag);
    EntityVector& getEntities();
    EntityVector& getEntities(const std::string& tag);
};