#pragma once

#include "Animation.hpp"

#include <fstream>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

typedef std::unordered_map<std::string, sf::Texture> TextureMap;
typedef std::unordered_map<std::string, sf::Font> FontMap;
typedef std::unordered_map<std::string, Animation> AnimationMap;
typedef std::unordered_map<std::string, sf::SoundBuffer> SoundMap;

class Assets
{
private:
    TextureMap      m_textures;
    FontMap         m_fonts;
    AnimationMap    m_animations;
    SoundMap        m_sounds;

    void addTexture(const std::string & name, const std::string & filePath);
    void addAnimation(const std::string & name, const std::string & textureName, size_t frames, size_t speed);
    void addFont(const std::string & name, const std::string & filePath);
    void addSound(const std::string & name, const std::string & filePath);

public:
    Assets() = default;

    void loadFromFile(const std::string & filePath);

    const sf::Texture & texture(const std::string & name) const;
    const Animation & animation(const std::string & name) const;
    const sf::Font & font(const std::string & name) const;
    const sf::SoundBuffer & sound(const std::string & name) const;
};