#pragma once

#include <string>

class Action
{
private:
    std::string m_name  = "NONE";
    std::string m_type  = "NONE";

public:
    Action() = default;
    Action(const std::string &name, const std::string &type);

    const std::string &name() const;
    const std::string &type() const;
    std::string toString() const;
};