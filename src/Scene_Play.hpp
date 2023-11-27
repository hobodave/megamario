#pragma once

#include "Scene.hpp"
#include "EntityManager.hpp"
#include "Vec2.hpp"

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

class Scene_Play : public Scene
{
    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

protected:

    std::shared_ptr<Entity> m_player;
    std::string             m_levelPath;
    PlayerConfig            m_playerConfig;
    bool                    m_drawTextures = true;
    bool                    m_drawCollision = false;
    bool                    m_drawGrid = false;
    const Vec2              m_gridSize { 64, 64 };
    sf::Text                m_gridText;

    void init(const std::string& levelPath);

    void loadLevel(const std::string& filename);

    void update();
    void spawnPlayer();
    void spawnBullet(std::shared_ptr<Entity> entity);

    void sMovement();
    void sLifespan();
    void sCollision();
    void sAnimation();
    void sDoAction(const Action& action);

    void sRender();
    void onEnd();
public:

    Scene_Play(GameEngine & gameEngine, const std::string& levelPath);

    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);


};