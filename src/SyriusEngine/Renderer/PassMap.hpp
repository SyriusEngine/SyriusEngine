#pragma once

#include "RenderGraphInclude.hpp"

namespace Syrius{

    class Pass;

    class PassMap{
    public:
        PassMap() = default;

        ~PassMap() = default;

        template<typename T, typename... Args>
        T* addPass(Args&&... args){
            SR_PRECONDITION(m_PassMap.find(PassIDGenerator<T>::getID()) == m_PassMap.end(), "Pass (%zul) already exists", PassIDGenerator<T>::getID());

            m_PassMap.insert({PassIDGenerator<T>::getID(), createResource<T>(std::forward<Args>(args)...)});
            auto pass = static_cast<T*>(m_PassMap[PassIDGenerator<T>::getID()].get());

            SR_POSTCONDITION(pass != nullptr, "Pass (%zul) could not be created", PassIDGenerator<T>::getID());
            return pass;
        }

        template<typename T>
        T* getPass(){
            SR_PRECONDITION(m_PassMap.find(PassIDGenerator<T>::getID()) != m_PassMap.end(), "Pass (%zul) not found", PassIDGenerator<T>::getID());

            return static_cast<T*>(m_PassMap[PassIDGenerator<T>::getID()].get());
        }

        auto begin() const{
            return m_PassMap.begin();
        }

        auto end() const{
            return m_PassMap.end();
        }

        inline size_t size() const{
            return m_PassMap.size();
        }

    private:
        std::unordered_map<PassID, Resource<Pass>> m_PassMap;

    };

}