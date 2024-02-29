#pragma once

#include "RenderGraphInclude.hpp"
#include "PassMap.hpp"

namespace Syrius{

    class Pass{
    public:
        Pass() = delete;

        virtual ~Pass();

        virtual void execute(PassMap& passMap) = 0;

        [[nodiscard]] inline const std::string& getName() const{
            return m_PassDesc.name;
        }

        [[nodiscard]] inline PassID getID() const{
            return m_PassDesc.id;
        }

        template<typename T>
        [[nodiscard]] inline bool is() const{
            return m_PassDesc.id == PassIDGenerator<T>::getID();
        }

        template<typename T>
        void addDependency(){
            m_PassDesc.dependencies.push_back(PassIDGenerator<T>::getID());
        }

        [[nodiscard]] inline const std::vector<PassID>& getDependencies() const{
            return m_PassDesc.dependencies;
        }

        [[nodiscard]] inline const PassDesc& getPassDesc() const{
            return m_PassDesc;
        }

    protected:
        Pass(ResourceView<Context>& context, const std::string& name, PassID id);

    protected:
        ResourceView<Context>& m_Context;

    private:
        PassDesc m_PassDesc;
    };
}