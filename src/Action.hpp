#pragma once

#include "Vec2.hpp"

#include <string>

class Action
{
private:
    std::string m_name  = "NONE";
    std::string m_type  = "NONE";
    Vec2        m_position = Vec2(0, 0);

public:
    Action() = default;
    Action(const std::string &name, const std::string &type);
    Action(const std::string &name, const std::string &type, const Vec2 &position);

    const std::string &name() const;
    const std::string &type() const;
    const Vec2 &position() const;
    std::string toString() const;
};