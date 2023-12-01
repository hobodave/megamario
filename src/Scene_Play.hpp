#pragma once

#include "Scene.hpp"
#include "EntityManager.hpp"
#include "Vec2.hpp"

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
    size_t                  m_jumpFrame = 0;

    Vec2                    m_mousePos;
    sf::CircleShape         m_mouseShape;

    void init(const std::string& levelPath);

    void loadLevel(const std::string& filename);

    void update();
    void onEnd();
    void spawnPlayer();
    void spawnBullet(std::shared_ptr<Entity> entity);
    // void hitBlock(std::shared_ptr<Entity> entity);

    Vec2 windowToWorld(const Vec2& window) const;

    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

    void sDoAction(const Action& action);

    void sDragAndDrop();
    void sMovement();
    void sLifespan();
    void sAnimation();
    void sCollision();
    void sRender();

public:

    Scene_Play(GameEngine & gameEngine, const std::string& levelPath);
};