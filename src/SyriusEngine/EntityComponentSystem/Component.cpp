#include "../../../include/SyriusEngine/EntityComponentSystem/Component.hpp"

namespace Syrius{

    std::unordered_map<std::type_index, ComponentID> ComponentBase::m_ComponentIDMap;

    const std::unordered_map<std::type_index, ComponentID>& ComponentBase::getComponentIDMap() {
        return m_ComponentIDMap;
    }
}
