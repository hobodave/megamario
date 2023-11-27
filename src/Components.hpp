#pragma once

#include "Animation.hpp"
#include "Vec2.hpp"

class Component
{
public:
    bool has = false;
};

class CTransform : public Component
{
public:

    Vec2 pos        { 0.0, 0.0 };
    Vec2 prevPos    { 0.0, 0.0 };
    Vec2 scale      { 1.0, 1.0 };
    Vec2 velocity   { 0.0, 0.0 };
    float angle     = 0.0;

    CTransform() = default;
    CTransform(const Vec2 & p) : pos(p) {}
    CTransform(const Vec2 & p, const Vec2 & velocity, const Vec2 & scale, float angle)
        : pos(p), prevPos(p), velocity(velocity), scale(scale), angle(angle) {}
};

class CLifespan : public Component
{
public:
    size_t lifespan     {0};
    size_t frameCreated {0};

    CLifespan() = default;
    CLifespan(size_t duration, size_t frameCreated)
        : lifespan(duration), frameCreated(frameCreated) {}
};

class CInput : public Component
{
public:
    bool up         { false };
    bool down       { false };
    bool left       { false };
    bool right      { false };
    bool shoot      { false };
    bool canShoot   { true };
    bool canJump    { true };

    CInput() = default;
};

class CBoundingBox : public Component
{
public:
    Vec2 size;
    Vec2 halfSize;

    CBoundingBox() = default;
    CBoundingBox(const Vec2 & size) : size(size), halfSize(size / 2.0) {}
};

class CAnimation : public Component
{
public:
    Animation animation;
    bool repeat = false;

    CAnimation() = default;
    CAnimation(const Animation & animation, bool repeat)
        : animation(animation), repeat(repeat) {}
};

class CGravity : public Component
{
public:
    float gravity { 0.0 };

    CGravity() = default;
    CGravity(float gravity) : gravity(gravity) {}
};

class CState : public Component
{
public:
    std::string state   { "standing" };

    CState() = default;
    CState(const std::string & state) : state(state) {}
};