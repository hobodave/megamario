#include "GameEngine.hpp"
#include "Assets.hpp"
#include "Scene_Play.hpp"
#include "Scene_Menu.hpp"

#include <iostream>

GameEngine::GameEngine(const std::string & path)
{
    init(path);
}

void GameEngine::init(const std::string & path)
{
    m_assets.loadFromFile(path);

    m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
    m_window.setFramerateLimit(60);

    changeScene("MENU", std::make_shared<Scene_Menu>(*this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning() const
{
    return m_running && m_window.isOpen();
}

sf::RenderWindow & GameEngine::window()
{
    return m_window;
}

void GameEngine::run()
{
    while (isRunning())
    {
        update();
    }
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            quit();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::X)
            {
                sf::Texture texture;
                texture.create(m_window.getSize().x, m_window.getSize().y);
                texture.update(m_window);

                if(texture.copyToImage().saveToFile("test.png"))
                {
                    std::cout << "screenshot saved to " << "test.png" << std::endl;
                }
                else
                {
                    std::cout << "failed to save screenshot" << std::endl;
                }
            }
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // if the current scene does not have an action associated with this key, skip the event
            if (currentScene()->actionMap().find(event.key.code) == currentScene()->actionMap().end())
            {
                continue;
            }

            // determine start or end action by whether it was key press or release
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

            // look up the action and send the action to the scene
            currentScene()->doAction(Action(currentScene()->actionMap().at(event.key.code), actionType));
        }

        auto mousePos = sf::Mouse::getPosition(m_window);
        Vec2 mousePosition = Vec2(mousePos.x, mousePos.y);

        if (event.type == sf::Event::MouseButtonPressed)
        {
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Left:   { currentScene()->doAction(Action("LEFT_CLICK", "START", mousePosition)); break; }
                case sf::Mouse::Middle: { currentScene()->doAction(Action("MIDDLE_CLICK", "START", mousePosition)); break; }
                case sf::Mouse::Right:  { currentScene()->doAction(Action("RIGHT_CLICK", "START", mousePosition)); break; }
                default: break;
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Left:   { currentScene()->doAction(Action("LEFT_CLICK", "END", mousePosition)); break; }
                case sf::Mouse::Middle: { currentScene()->doAction(Action("MIDDLE_CLICK", "END", mousePosition)); break; }
                case sf::Mouse::Right:  { currentScene()->doAction(Action("RIGHT_CLICK", "END", mousePosition)); break; }
                default: break;
            }
        }

        if (event.type == sf::Event::MouseMoved)
        {
            currentScene()->doAction(Action("MOUSE_MOVE", "START", Vec2(event.mouseMove.x, event.mouseMove.y)));
        }
    }
}

void GameEngine::changeScene(const std::string & sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    if (scene)
    {
        m_sceneMap[sceneName] = scene;
        m_currentScene = sceneName;
    }
    else
    {
        if (endCurrentScene)
        {
            m_sceneMap.erase(m_currentScene);
        }

        m_currentScene = sceneName;
    }
}

void GameEngine::quit()
{
    m_running = false;
}

void GameEngine::update()
{
    sUserInput();
    currentScene()->update();
}

const Assets & GameEngine::assets() const
{
    return m_assets;
}
