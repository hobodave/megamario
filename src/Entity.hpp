#pragma once

#include "Components.hpp"

#include <tuple>
#include <string>

class EntityManager;

typedef std::tuple<
    CTransform,
    CLifespan,
    CInput,
    CBoundingBox,
    CAnimation,
    CGravity,
    CState
> ComponentTuple;

class Entity
{
    friend class EntityManager;

private:

    bool                m_active    { true };
    std::string         m_tag       { "default" };
    size_t              m_id        { 0 };
    ComponentTuple      m_components;

    // Constructor is private so that only the EntityManager can create entities
    Entity(const size_t & id, const std::string & tag);

public:

    void                destroy();
    size_t              id()        const;
    bool                active()    const;
    const std::string & tag()       const;

    template<typename T>
    bool hasComponent() const
    {
        return getComponent<T>().has;
    }

    template<typename T, typename... TArgs>
    T & addComponent(TArgs&&... args)
    {
        auto & component = getComponent<T>();
        component = T(std::forward<TArgs>(args)...);
        component.has = true;
        return component;
    }

    template<typename T>
    T & getComponent()
    {
        return std::get<T>(m_components);
    }

    template<typename T>
    const T & getComponent() const
    {
        return std::get<T>(m_components);
    }

    template<typename T>
    void removeComponent()
    {
        getComponent<T>() = T();
    }
};