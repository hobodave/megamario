#include "Scene_Menu.hpp"

Scene_Menu::Scene_Menu(GameEngine & m_game)
    : Scene(m_game)
{
    init();
}

void Scene_Menu::init()
{
    m_title = "Definitely Not Mario";
    m_menuStrings.push_back("Level 1");
    m_menuStrings.push_back("Level 2");
    m_menuStrings.push_back("Level 3");

    m_levelPaths = { "assets/levels/level1.txt", "assets/levels/level2.txt", "assets/levels/level3.txt" };

    m_menuText.setFont(m_game.assets().font("Tech"));
    m_menuText.setCharacterSize(24);
    m_menuText.setFillColor(sf::Color::White);
    m_menuText.setOutlineColor(sf::Color::Black);
    m_menuText.setOutlineThickness(2);
}

void Scene_Menu::update()
{
    sRender();
}

void Scene_Menu::onEnd()
{
}

void Scene_Menu::sDoAction(const Action& action)
{
    if (action.name() == "UP")
    {
        m_selectedMenuIndex = (m_selectedMenuIndex + m_menuStrings.size() - 1) % m_menuStrings.size();
    }
    else if (action.name() == "DOWN")
    {
        m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
    }
    else if (action.name() == "SELECT")
    {
        // m_engine->changeScene("PLAY", std::make_shared<Scene_Play>(m_engine, m_levelPaths[m_selectedMenuIndex]));
    }
}

void Scene_Menu::sRender()
{
    m_game.window().clear(sf::Color::Black);
    m_menuText.setString(m_title);
    m_menuText.setPosition(640 - m_menuText.getLocalBounds().width / 2, 100);
    m_game.window().draw(m_menuText);

    for (size_t i = 0; i < m_menuStrings.size(); ++i)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setPosition(640 - m_menuText.getLocalBounds().width / 2, 200 + i * 50);
        m_game.window().draw(m_menuText);
    }

    m_menuText.setString(">");
    m_menuText.setPosition(640 - m_menuText.getLocalBounds().width / 2 - 50, 200 + m_selectedMenuIndex * 50);
    m_game.window().draw(m_menuText);
    m_game.window().display();
}