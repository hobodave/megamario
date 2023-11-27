#pragma once

#include "Vec2.hpp"

#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
private:
    sf::Sprite  m_sprite;
    size_t      m_frameCount    = 1;
    size_t      m_currentFrame  = 0;
    size_t      m_speed         = 0;
    Vec2        m_size          = { 1, 1 };
    std::string m_name = "none";

public:
    Animation();
    Animation(const std::string &name, const sf::Texture &texture);
    Animation(const std::string &name, const sf::Texture &texture, size_t frameCount, size_t speed);

    void update();
    bool hasEnded() const;
    const std::string &name() const;
    const Vec2 &size() const;
    sf::Sprite &sprite();
};