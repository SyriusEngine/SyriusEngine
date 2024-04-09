#pragma once

#include "ECSPrimitives.hpp"

namespace Syrius{

    class SR_API ComponentBase{
    public:
        static const std::unordered_map<std::type_index, ComponentID>& getComponentIDMap();

    protected:
        static std::unordered_map<std::type_index, ComponentID> m_ComponentIDMap;

    };

    template<typename ComponentType>
    class SR_API Component: public ComponentBase{
    public:
        virtual ~Component() = default;

        static ComponentID getID(){
            static ComponentID id = createID();
            return id;
        }

    private:
        static ComponentID createID(){
            ComponentID id = generateID();
            m_ComponentIDMap[typeid(ComponentType)] = id;
            return id;
        }

    };

}