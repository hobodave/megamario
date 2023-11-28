#include "Scene_Play.hpp"
#include "Scene_Menu.hpp"
#include "Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.hpp"
#include "Components.hpp"
#include "Action.hpp"

#include <fstream>
#include <iostream>
#include <string>

Scene_Play::Scene_Play(GameEngine & gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath)
{
    init(m_levelPath);
}

void Scene_Play::init(const std::string &levelPath)
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURES");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");

    registerAction(sf::Keyboard::Left, "LEFT");
    registerAction(sf::Keyboard::A, "LEFT");

    registerAction(sf::Keyboard::Right, "RIGHT");
    registerAction(sf::Keyboard::D, "RIGHT");

    registerAction(sf::Keyboard::Up, "UP");
    registerAction(sf::Keyboard::W, "UP");

    registerAction(sf::Keyboard::Down, "DOWN");
    registerAction(sf::Keyboard::S, "DOWN");

    registerAction(sf::Keyboard::Space, "SHOOT");
    // TODO: Register all other gameplay Actions

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game.assets().font("Tech"));

    loadLevel(levelPath);
};

////////////////////////////////////////////////////////////////////////////////
/// @brief Converts a grid position to pixel position for the center of the entity
/// @param gridX
/// @param gridY
/// @param entity
/// @return
////////////////////////////////////////////////////////////////////////////////
Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    Vec2 animationMidpoint = entity->getComponent<CAnimation>().animation.size() / 2.0;
    Vec2 gridOrigin = Vec2(gridX * m_gridSize.x, m_game.window().getSize().y - (gridY * m_gridSize.y));

    return Vec2(gridOrigin.x + animationMidpoint.x, gridOrigin.y - animationMidpoint.y);
};

void Scene_Play::loadLevel(const std::string &filename)
{
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    std::ifstream file(filename);
    std::string directive;

    while (file.good())
    {
        file >> directive;

        if (directive == "Tile")
        {
            std::string name;
            float gridX, gridY;
            file >> name >> gridX >> gridY;
            auto tile = m_entityManager.addEntity("tile");
            tile->addComponent<CAnimation>(m_game.assets().animation(name), true);
            tile->addComponent<CTransform>(gridToMidPixel(gridX, gridY, tile));

            if (name == "Ground" || name == "Brick" || name == "Question" || name == "Block")
            {
                tile->addComponent<CBoundingBox>(Vec2(64, 64));
            }
            else if (name == "PipeSmall")
            {
                tile->addComponent<CBoundingBox>(Vec2(128, 138));
            }
        }
        else if (directive == "Player")
        {
            float gridX, gridY, boxW, boxH, speedX, speedY, speedMax, gravity;
            std::string bulletAnimation;

            file >> gridX >> gridY
                 >> boxW >> boxH
                 >> speedX >> speedY >> speedMax
                 >> gravity >> bulletAnimation;

            m_playerConfig.X = gridX;
            m_playerConfig.Y = gridY;
            m_playerConfig.CX = boxW;
            m_playerConfig.CY = boxH;
            m_playerConfig.SPEED = speedX;
            m_playerConfig.MAXSPEED = speedMax;
            m_playerConfig.JUMP = speedY;
            m_playerConfig.GRAVITY = gravity;
            m_playerConfig.WEAPON = bulletAnimation;

            spawnPlayer();
        }
    }

    // NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
    //       Components are now returned as references rather than pointers
    //       If you do not specify a reference variable type, it will COPY the component
    //       Here is an example:
    //
    //       This will COPY the transform into the variable 'transform1' - it is INCORRECT
    //       auto transform1 = entity->getComponent<CTransform>();
    //
    //       This will REFERENCE the transform into the variable 'transform2' - it is CORRECT
    //       auto& transform2 = entity->getComponent<CTransform>();
}

void Scene_Play::spawnPlayer()
{
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game.assets().animation("Stand"), true);
    m_player->addComponent<CTransform>(gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player));
    // m_player->getComponent<CTransform>().scale = Vec2(0.75f, 0.75f);
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
    m_player->addComponent<CInput>();
    m_player->addComponent<CState>("ground");
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
    auto bullet = m_entityManager.addEntity("bullet");
    float bulletDirection = entity->getComponent<CTransform>().scale.x < 0 ? -1.0f : 1.0f;
    Vec2 bulletVelocity = Vec2(bulletDirection * 10.0f, 0.0f);

    bullet->addComponent<CAnimation>(m_game.assets().animation(m_playerConfig.WEAPON), true);
    bullet->addComponent<CTransform>(entity->getComponent<CTransform>().pos, bulletVelocity);
    bullet->addComponent<CBoundingBox>(Vec2(16, 12));
    bullet->addComponent<CLifespan>(180, m_currentFrame);
}

void Scene_Play::update()
{
    m_entityManager.update();

    if (!m_paused)
    {
        m_currentFrame++;
        sMovement();
        sLifespan();
        sCollision();
        sAnimation();
    }

    sRender();
}

void Scene_Play::sMovement()
{
    auto& playerTransform = m_player->getComponent<CTransform>();
    playerTransform.velocity.x = 0;

    for (auto e : m_entityManager.getEntities())
    {
        auto& entityTransform = e->getComponent<CTransform>();
        entityTransform.prevPos = entityTransform.pos;

        if (e->hasComponent<CInput>())
        {
            auto& input = e->getComponent<CInput>();

            if (input.left)
            {
                entityTransform.velocity.x -= m_playerConfig.SPEED;
                entityTransform.scale.x = -1;
            }
            else if (input.right)
            {
                entityTransform.velocity.x += m_playerConfig.SPEED;
                entityTransform.scale.x = 1;
            }
            else if (input.up)
            {
                entityTransform.velocity.y -= m_playerConfig.JUMP;
            }
        }

        if (e->hasComponent<CGravity>())
        {
            entityTransform.velocity.y += e->getComponent<CGravity>().gravity;
        }

        // Enforce speed limits
        entityTransform.velocity.x = std::max(-m_playerConfig.MAXSPEED, std::min(m_playerConfig.MAXSPEED, entityTransform.velocity.x));
        entityTransform.velocity.y = std::max(-m_playerConfig.MAXSPEED, std::min(m_playerConfig.MAXSPEED, entityTransform.velocity.y));

        entityTransform.pos += entityTransform.velocity;
    }
}

void Scene_Play::sLifespan()
{
    // Check for entities with a CLifespan component and destroy them if they are too old
    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CLifespan>())
        {
            auto& lifespan = e->getComponent<CLifespan>();

            if (m_currentFrame - lifespan.frameCreated >= lifespan.lifespan)
            {
                e->destroy();
            }
        }
    }
}

void Scene_Play::sCollision()
{
    // Check for bullet collisions
    for (auto bullet : m_entityManager.getEntities("bullet"))
    {
        // assume CBoundingBox exists
        auto& bulletBox = bullet->getComponent<CBoundingBox>();
        auto& bulletTransform = bullet->getComponent<CTransform>();

        // Check all tiles for collision
        for (auto tile : m_entityManager.getEntities("tile"))
        {
            if (!tile->hasComponent<CBoundingBox>())
            {
                continue;
            }

            auto& tileBox = bullet->getComponent<CBoundingBox>();
            auto& tileTransform = bullet->getComponent<CTransform>();

            Vec2 overlap = Physics::GetOverlap(bullet, tile);

            if (overlap.x <= 0 || overlap.y <= 0)
            {
                continue;
            }

            // Bullet overlaps with tile and we don't care how, destroy it and possibly the tile if it's a brick
            bullet->destroy();

            if (tile->getComponent<CAnimation>().animation.name() == "Brick")
            {
                tile->removeComponent<CBoundingBox>();
                tile->addComponent<CAnimation>(m_game.assets().animation("Explosion"), false);
                tile->addComponent<CState>("dead");
            }
        }
    }

    auto& playerTransform = m_player->getComponent<CTransform>();

    // Check for player-tile collisions
    for (auto e : m_entityManager.getEntities("tile"))
    {
        // Ignore decorative non-colliding tiles
        if (e->hasComponent<CBoundingBox>())
        {
            auto& tileBox = e->getComponent<CBoundingBox>();
            auto& tileTransform = e->getComponent<CTransform>();

            auto& playerBox = m_player->getComponent<CBoundingBox>();

            Vec2 overlap = Physics::GetOverlap(m_player, e);

            if (overlap.x <= 0 || overlap.y <= 0)
            {
                continue;
            }

            Vec2 prevOverlap = Physics::GetPreviousOverlap(m_player, e);

            // Check for x-axis collision
            if (overlap.x > 0)
            {
                // if player is to the left of tile
                if (prevOverlap.y > 0 && playerTransform.pos.x < tileTransform.pos.x)
                {
                    // set player to be to the left of tile
                    playerTransform.pos.x -= overlap.x;
                }
                // if player is to the right of tile
                else if (prevOverlap.y > 0 && playerTransform.pos.x > tileTransform.pos.x)
                {
                    // set player to be to the right of tile
                    playerTransform.pos.x += overlap.x;
                }
            }

            // Check for y-axis collision
            if (overlap.y > 0)
            {
                // if player is above tile
                if (prevOverlap.x > 0 && playerTransform.pos.y < tileTransform.pos.y)
                {
                    // set player to be on top of tile
                    playerTransform.pos.y -= overlap.y;
                    playerTransform.velocity.y = 0;
                    if (playerTransform.velocity.x == 0)
                    {
                        m_player->getComponent<CState>().state = "ground";
                    }
                    else
                    {
                        m_player->getComponent<CState>().state = "run";
                    }

                    m_player->getComponent<CInput>().canJump = true;
                }
                // if player is below tile
                else if (prevOverlap.x > 0 && playerTransform.pos.y > tileTransform.pos.y)
                {
                    // set player to be below tile
                    playerTransform.pos.y += overlap.y;
                    playerTransform.velocity.y = 0;
                }
            }
        }
    }

    // Prevent player from going off the left side of the screen
    if (playerTransform.pos.x < 32.0f)
    {
        playerTransform.pos.x = 32.0f;
    }

    // Respawn player if they fall off the bottom of the screen
    if (playerTransform.pos.y > height())
    {
        m_player->destroy();
        spawnPlayer();
    }
}

void Scene_Play::sDoAction(const Action &action)
{
    auto& playerInput = m_player->getComponent<CInput>();

    if (action.type() == "START")
    {
        if (action.name() == "TOGGLE_TEXTURES")
        {
            m_drawTextures = !m_drawTextures;
        }
        else if (action.name() == "TOGGLE_COLLISION")
        {
            m_drawCollision = !m_drawCollision;
        }
        else if (action.name() == "TOGGLE_GRID")
        {
            m_drawGrid = !m_drawGrid;
        }
        else if (action.name() == "PAUSE")
        {
            setPaused(!m_paused);
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
        else if (action.name() == "LEFT")
        {
            playerInput.left = true;
            m_player->getComponent<CState>().state = "run";
        }
        else if (action.name() == "RIGHT")
        {
            playerInput.right = true;
            m_player->getComponent<CState>().state = "run";
        }
        else if (action.name() == "UP")
        {
            if (playerInput.canJump)
            {
                playerInput.up = true;
                playerInput.canJump = false;
                m_player->getComponent<CState>().state = "air";
            } else {
                playerInput.up = false;
            }
        }
        else if (action.name() == "DOWN")
        {
            playerInput.down = true;
        }
        else if (action.name() == "SHOOT")
        {
            if (playerInput.canShoot)
            {
                playerInput.shoot = true;
                playerInput.canShoot = false;
                spawnBullet(m_player); // Does this belong here or in sMovement?
            }
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "UP")
        {
            playerInput.up = false;
        }
        else if (action.name() == "DOWN")
        {
            playerInput.down = false;
        }
        else if (action.name() == "LEFT")
        {
            playerInput.left = false;
        }
        else if (action.name() == "RIGHT")
        {
            playerInput.right = false;
        }
        else if (action.name() == "SHOOT")
        {
            playerInput.shoot = false;
            playerInput.canShoot = true;
        }
    }
}

void Scene_Play::sAnimation()
{
    if (m_player->getComponent<CState>().state == "air")
    {
        m_player->addComponent<CAnimation>(m_game.assets().animation("Air"), true);
    }
    else if (m_player->getComponent<CState>().state == "run")
    {
        if (m_player->getComponent<CAnimation>().animation.name() != "Run")
        {
            m_player->addComponent<CAnimation>(m_game.assets().animation("Run"), true);
        }
    }
    else if (m_player->getComponent<CState>().state == "ground")
    {
        m_player->addComponent<CAnimation>(m_game.assets().animation("Stand"), true);
    }

    for (auto e : m_entityManager.getEntities())
    {
        if (!e->hasComponent<CAnimation>())
        {
            continue;
        }

        auto& animationComponent = e->getComponent<CAnimation>();
        animationComponent.animation.update();

        if (!animationComponent.repeat && animationComponent.animation.hasEnded())
        {
            e->destroy();
        }
    }
};

void Scene_Play::onEnd()
{
    m_game.changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
};

void Scene_Play::sRender()
{
    // color the background darker so you know when the game is paused
    if (!m_paused)
    {
        m_game.window().clear(sf::Color(107, 135, 255));
    }
    else
    {
        m_game.window().clear(sf::Color(75, 75, 255));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto &playerPosition = m_player->getComponent<CTransform>().pos;
    sf::View view = m_game.window().getView();
    float windowCenterX = std::max(static_cast<float>(m_game.window().getSize().x / 2.0), playerPosition.x);
    view.setCenter(windowCenterX, m_game.window().getSize().y - view.getCenter().y);
    m_game.window().setView(view);

    // draw all entity textures and animations
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities())
        {
            auto &transform = e->getComponent<CTransform>();

            if (e->hasComponent<CAnimation>())
            {
                auto &animation = e->getComponent<CAnimation>().animation;
                animation.sprite().setRotation(transform.angle);
                animation.sprite().setPosition(transform.pos.x, transform.pos.y);
                animation.sprite().setScale(transform.scale.x, transform.scale.y);
                m_game.window().draw(animation.sprite());
            }
        }
    }

    // draw all entity bounding boxes
    if (m_drawCollision)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto &transform = e->getComponent<CTransform>();
                auto &box = e->getComponent<CBoundingBox>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                m_game.window().draw(rect);
            }
        }
    }

    // draw the grid
    if (m_drawGrid)
    {
        float leftX = m_game.window().getView().getCenter().x - width() / 2.0;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX & (int)m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x)
        {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y = 0; y <= height(); y += m_gridSize.y)
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string yCell = std::to_string((int)(y-1) / (int)m_gridSize.y);
                size_t previousSize = m_gridText.getCharacterSize();
                m_gridText.setCharacterSize(6);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 2, height() - y + 4);
                m_game.window().draw(m_gridText);
                m_gridText.setCharacterSize(previousSize);
            }
        }
    }

    // Draw PAUSED if paused
    if (m_paused)
    {
        size_t previousSize = m_gridText.getCharacterSize();
        m_gridText.setCharacterSize(36);
        m_gridText.setString("PAUSED");
        sf::FloatRect textRect = m_gridText.getLocalBounds();
        m_gridText.setOrigin(textRect.left + textRect.width/2.0f,
                textRect.top  + textRect.height/2.0f);
        m_gridText.setPosition(width() / 2.0, height() / 2.0);
        m_game.window().draw(m_gridText);
        m_gridText.setCharacterSize(previousSize);
    }

    m_game.window().display();
};