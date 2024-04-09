#pragma once

#include "../Core/SyriusEngineInclude.hpp"
#include <any>
#include <typeindex>

namespace Syrius{

    typedef UID EntityID;
    typedef UID ComponentID;
    typedef size_t Offset;

    typedef std::vector<std::any> ComponentData;
    typedef std::unordered_map<ComponentID, Offset> Entity;

}