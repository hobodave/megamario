#include "Assets.hpp"
#include <cassert>

void Assets::loadFromFile(const std::string & filePath)
{
    std::ifstream file(filePath);
    std::string directive;

    while (file.good())
    {
        file >> directive;

        if (directive == "Texture")
        {
            std::string name, path;
            file >> name >> path;
            addTexture(name, path);
        }
        else if (directive == "Animation")
        {
            std::string name, texture;
            size_t frames, speed;
            file >> name >> texture >> frames >> speed;
            addAnimation(name, texture, frames, speed);
        }
        else if (directive == "Font")
        {
            std::string name, path;
            file >> name >> path;
            addFont(name, path);
        }
        else
        {
            std::cerr << "Unknown Asset Type: " << directive << std::endl;
        }
    }
}

void Assets::addTexture(const std::string & name, const std::string & filePath)
{
    m_textures[name] = sf::Texture();

    if (!m_textures[name].loadFromFile(filePath))
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        m_textures.erase(name);
    }
    else
    {
        m_textures[name].setSmooth(true);
        std::cout << "Loaded texture: " << filePath << std::endl;
    }
}

const sf::Texture & Assets::texture(const std::string & name) const
{
    if (m_textures.find(name) == m_textures.end())
    {
        std::cerr << "Texture not found: " << name << std::endl;
        return m_textures.at("Missing");
    }

    return m_textures.at(name);
}

void Assets::addAnimation(const std::string & name, const std::string & textureName, size_t frames, size_t speed)
{
    m_animations[name] = Animation(name, texture(textureName), frames, speed);
}

const Animation & Assets::animation(const std::string & name) const
{
    if (m_animations.find(name) == m_animations.end())
    {
        std::cerr << "Animation not found: " << name << std::endl;
        assert(m_animations.find(name) != m_animations.end());
    }
    return m_animations.at(name);
}

void Assets::addFont(const std::string & name, const std::string & filePath)
{
    m_fonts[name] = sf::Font();

    if (!m_fonts[name].loadFromFile(filePath))
    {
        std::cerr << "Failed to load font: " << filePath << std::endl;
        m_fonts.erase(name);
    }
    else
    {
        std::cout << "Loaded font: " << filePath << std::endl;
    }
}

const sf::Font & Assets::font(const std::string & name) const
{
    assert(m_fonts.find(name) != m_fonts.end());
    return m_fonts.at(name);
}