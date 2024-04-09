#include "../../../include/SyriusEngine/EntityComponentSystem/Component.hpp"
#include "../../../include/SyriusEngine/EntityComponentSystem/EntityComponentSystem.hpp"


namespace Syrius{

    Syrius::EntityComponentSystem::EntityComponentSystem() = default;

    EntityComponentSystem::~EntityComponentSystem() = default;

    EntityID EntityComponentSystem::createEntity() {
        EntityID entityID = generateID();
        m_Entities.insert({entityID, Entity()});
        return entityID;
    }
}
