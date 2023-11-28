#include "Scene_Play.hpp"
#include "Scene_Menu.hpp"
#include "Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.hpp"
#include "Components.hpp"
#include "Action.hpp"

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

    // TODO: Register all other gameplay Actions

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game.assets().font("Tech"));

    loadLevel(levelPath);
};

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    // TODO: This function takes a grid (x,y) position and an Entity
    //       Returns a Vec2 indicating where the CENTER position of the Entity should be
    //       You must use the Entity's Animation size to position it correctly
    //       The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    //       The bottom-left corner of the Animation should align with the bottom-left of the grid cell

    return Vec2(0, 0);
};

void Scene_Play::loadLevel(const std::string &filename)
{
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    // TODO: read in the level file and add the appropriate entities
    //       use the PlayerConfig struct m_playerConfig to store player properties
    //       this struct is defined at the top of Scene_Play.hpp

    // NOTE: all of the code below is sample code which shows you how to
    //       set up and use entities with the new syntax, it should be removed

    spawnPlayer();

    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT: always add the CAnimation component first so that gridToMidPixel works
    brick->addComponent<CAnimation>(m_game.assets().animation("Brick"), true);
    brick->addComponent<CTransform>(Vec2(96, 480));
    brick->getComponent<CTransform>().scale = Vec2(2, 2);
    // NOte:: Your final code should position the entity with the grid x,y position read from the file:
    // brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));

    if (brick->getComponent<CAnimation>().animation.name() == "Brick")
    {
        std::cout << "This could be a good way of identifying if a tile is a brick!\n";
    }

    auto block = m_entityManager.addEntity("tile");
    block->addComponent<CAnimation>(m_game.assets().animation("Block"), true);
    block->addComponent<CTransform>(Vec2(224, 480));
    block->getComponent<CTransform>().scale = Vec2(0.5f, 0.5f);
    // add a bounding box, this will now show up if we press the C key
    Vec2 animationSize = m_game.assets().animation("Block").size();
    Vec2 scale = block->getComponent<CTransform>().scale;
    block->addComponent<CBoundingBox>(Vec2(animationSize.x * scale.x, animationSize.y * scale.y));

    auto question = m_entityManager.addEntity("tile");
    question->addComponent<CAnimation>(m_game.assets().animation("Question"), true);
    question->addComponent<CTransform>(Vec2(352, 480));
    question->getComponent<CTransform>().scale = Vec2(2.0f, 2.0f);

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
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game.assets().animation("Stand"), true);
    m_player->addComponent<CTransform>(Vec2(224, 352));
    m_player->getComponent<CTransform>().scale = Vec2(0.75, 0.75);
    m_player->addComponent<CBoundingBox>(Vec2(48, 48));
    m_player->addComponent<CGravity>(0.1);

    // TODO: be sure to add the remaining components to the player
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
}

void Scene_Play::update()
{
    m_entityManager.update();

    // TODO: implement pause functionality

    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();
}

void Scene_Play::sMovement()
{
    // TODO: Implement player movement / jumping based on its CInput component
    // Vec2 playerVelocity{0, m_player->getComponent<CTransform>().velocity.y};

    // // if (m_player->getComponent<CInput>().up)
    // // {
    // //     playerVelocity.y = -3;
    // // }

    // m_player->getComponent<CTransform>().velocity = playerVelocity;

    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CGravity>())
        {
            // e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
            // if player is moving faster than max speed, set it to max speed
        }

        e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
    }
    // TODO: Implement gravity's effect on the player
    // TODO: Implement the maximum player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face left/right
}

void Scene_Play::sLifespan()
{
    // TODO: Check lifespan of entities that have them, and destroy them if they go over
}

void Scene_Play::sCollision()
{
    // REMEMBER: SFML's (0,) position is the TOP-LEFT corner of the screen
    //           This means jumping will have a negative y-component
    //           Gravity will have a positive y-component
    //           Also, something BELOW something else will have a y value GREATER than the other
    //           Also, something ABOVE something else will have a y value LESS than the other

    // TODO: Implement Physics::GetOverlap(), use it inside this function

    // TODO: Implment bullet / tile collisions
    //       Destroy the tile if it has a Brick animation
    // TODO: Implement player / tile collisions and resolutions
    //       Update the CState component of the player to store whether
    //       it is on the ground or not. This will be used by the Animation system
    // TODO: Check to see if the player has fallen down a hole (y > height())
    // TODO: Don't let the player walk off the left side of the map
}

void Scene_Play::sDoAction(const Action &action)
{
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
            m_player->getComponent<CInput>().left = true;
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = true;
        }
        else if (action.name() == "UP")
        {
            m_player->getComponent<CInput>().up = true;
        }
        else if (action.name() == "DOWN")
        {
            m_player->getComponent<CInput>().down = true;
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "UP")
        {
            m_player->getComponent<CInput>().up = false;
        }
        else if (action.name() == "DOWN")
        {
            m_player->getComponent<CInput>().down = false;
        }
        else if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = false;
        }
    }
}

void Scene_Play::sAnimation()
{
    // TODO: Complete Animation class code first

    if (m_player->getComponent<CState>().state == "air")
    {
        // m_player->addComponent<CAnimation>(m_game.assets().animation("Air"));
    }

    // TODO: set the animation of the player based on its CState component
    // TODO: for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
    //       if the animation is not repeated, and it has ended, destroy the entity
};

void Scene_Play::onEnd(){
    m_game.changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
    // TODO: When the scene ends, change back to the MENU scene
    //       use m_game.changeScene(correct params);
};

void Scene_Play::sRender()
{
    // color the background darker so you know when the game is paused
    // if (!m_paused)
    // {
    //     m_game.window().clear(sf::Color(100, 100, 255));
    // }
    // else
    // {
    //     m_game.window().clear(sf::Color(50, 50, 100));
    // }

    m_game.window().clear(sf::Color(0x47, 0x93, 0xcd));

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
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                size_t previousSize = m_gridText.getCharacterSize();
                m_gridText.setCharacterSize(6);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 2, height() - y + 4);
                m_game.window().draw(m_gridText);
                m_gridText.setCharacterSize(previousSize);
            }
        }
    }

    m_game.window().display();
};