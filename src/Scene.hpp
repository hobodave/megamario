#pragma once

#include "Action.hpp"
#include "EntityManager.hpp"
#include "GameEngine.hpp"

#include <SFML/Audio.hpp>
#include <memory>

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{
protected:
    GameEngine &        m_game;
    EntityManager       m_entityManager;
    ActionMap           m_actionMap;
    bool                m_paused = false;
    bool                m_hasEnded = false;
    size_t              m_currentFrame = 0;
    sf::Sound           m_sound;


    virtual void onEnd() = 0;
    void setPaused(bool paused);

public:

    Scene(GameEngine & gameEngine);

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;

    virtual void doAction(const Action& action);
    void simulate(const size_t frameCount);
    void registerAction(int inputKey, const std::string& actionName);

    size_t width() const;
    size_t height() const;
    size_t currentFrame() const;

    bool hasEnded() const;
    const ActionMap& actionMap() const;
    void drawLine(const Vec2& p1, const Vec2& p2);
};