#include "EntityManager.hpp"
#include <vector>

void EntityManager::update()
{
    for (auto& entity : m_entitiesToAdd)
    {
        m_entities.push_back(entity);
        m_entityMap[entity->tag()].push_back(entity);
    }

    removeDeadEntities(m_entities);

    for (auto& [tag, entities] : m_entityMap)
    {
        removeDeadEntities(entities);
    }

    m_entitiesToAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_entityCount++, tag));
    m_entitiesToAdd.push_back(entity);
    return entity;
}

EntityVector& EntityManager::getEntities()
{
    return m_entities;
}

EntityVector& EntityManager::getEntities(const std::string& tag)
{
    return m_entityMap[tag];
}

void EntityManager::removeDeadEntities(EntityVector& entities)
{
    auto it = std::remove_if(entities.begin(),
                    entities.end(),
                    [](std::shared_ptr<Entity> e) { return !e->active(); });
    entities.erase(it, entities.end());
}