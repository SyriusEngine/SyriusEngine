#pragma once

#include "Component.hpp"

namespace Syrius{

    class SR_API EntityComponentSystem{
    public:
        EntityComponentSystem();

        ~EntityComponentSystem();

        EntityID createEntity();

        [[nodiscard]] inline bool hasEntity(EntityID entityID) const { return m_Entities.find(entityID) != m_Entities.end(); }

        [[nodiscard]] inline const std::unordered_map<EntityID, Entity>& getEntities() const { return m_Entities; }

        template<typename C, typename... Args>
        void addComponent(EntityID entityID, Args&&... args) {
            SR_PRECONDITION(m_Entities.find(entityID) != m_Entities.end(), "Entity: %lld does not exists", entityID);
            SR_PRECONDITION(!hasComponent<C>(entityID), "Entity: %lld already has component of type: %s", entityID, typeid(C).name());

            auto cID = Component<C>::getID();
            if (m_ComponentMap.find(cID) == m_ComponentMap.end()){
                m_ComponentMap.insert({cID, std::vector<std::any>()});
            }
            auto& data = m_ComponentMap[cID];
            data.emplace_back(C{std::forward<Args>(args)...});
            Offset offset = data.size() - 1;
            m_Entities[entityID].insert({cID, offset});
            m_ReverseLookupMap[cID].insert({offset, entityID});
        }

        template<typename C>
        bool hasComponent(EntityID entityID) {
            SR_PRECONDITION(m_Entities.find(entityID) != m_Entities.end(), "Entity: %lld does not exists", entityID);

            auto cID = Component<C>::getID();
            return m_Entities[entityID].find(cID) != m_Entities[entityID].end();
        }

        template<typename C>
        C& getComponent(EntityID entityID){
            SR_PRECONDITION(m_Entities.find(entityID) != m_Entities.end(), "Entity: %lld does not exists", entityID);
            SR_PRECONDITION(hasComponent<C>(entityID), "Entity: %lld does not have component of type: %s", entityID, typeid(C).name());

            auto cID = Component<C>::getID();
            auto componentOffset = m_Entities[entityID][cID];
            return std::any_cast<C&>(m_ComponentMap[cID][componentOffset]);
        }

        template<typename C>
        void removeComponent(EntityID entityID) {
            SR_PRECONDITION(m_Entities.find(entityID) != m_Entities.end(), "Entity: %lld does not exists", entityID);
            SR_PRECONDITION(hasComponent<C>(entityID), "Entity: %lld does not have component of type: %s", entityID, typeid(C).name());

            /*
             * Component data is stored contiguously in a vector, so we can't just remove the component from the vector.
             * Each entity has a map of components, the map stores for each component the associated offset in the vector.
             * Removing a component from an entity means removing the component from the vector and updating the offset of the other components.
             * This can be done by swapping the component to remove with the last component in the vector and updating the offset of the last component.
             */
            auto cID = Component<C>::getID();
            auto cOffset = m_Entities[entityID][cID];
            auto& cData = m_ComponentMap[cID];

            if (cOffset != cData.size() - 1) {
                // last component in the vector belongs to some entity, we need to update the offset of that entity
                auto toUpdateEntityID = m_ReverseLookupMap[cID][cData.size() - 1];
                m_Entities[toUpdateEntityID][cID] = cOffset;
                m_ReverseLookupMap[cID][cOffset] = toUpdateEntityID;
                m_ReverseLookupMap[cID].erase(cData.size() - 1);

                // swap the component to remove with the last component in the vector
                std::swap(cData[cOffset], cData.back());
            }
            cData.pop_back();

            m_Entities[entityID].erase(cID);
        }

    private:
        typedef std::unordered_map<Offset, EntityID> ReverseLookup;

    private:
        std::unordered_map<EntityID, Entity> m_Entities;
        std::unordered_map<ComponentID, ComponentData> m_ComponentMap;
        std::unordered_map<ComponentID, ReverseLookup> m_ReverseLookupMap;
    };

}