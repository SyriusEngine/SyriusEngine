#pragma once

#include "Pass.hpp"
#include "PassMap.hpp"
#include "../RendererInclude.hpp"

namespace Syrius{

    typedef std::vector<PassID> Dependencies;
    typedef std::unordered_map<PassID, Dependencies> AdjacencyList;

    class RenderGraph{
    public:
        RenderGraph();

        ~RenderGraph();

        template<typename T, typename... Args>
        T* addPass(Args&&... args){
            auto pass = m_PassMap.addPass<T>(std::forward<Args>(args)...);

            m_PassDescs.insert({PassIDGenerator<T>::getID(), pass->getPassDesc()});
            return pass;
        }

        template<typename T>
        T* getPass(){
            return m_PassMap.getPass<T>();
        }

        template<typename T>
        bool hasPass(){
            return m_PassMap.hasPass<T>();
        }

        void execute();

        /**
         * @brief Checks for every pass if its dependencies are also inside the graph
         */
        void validate();

        /**
         * @brief Topologically sorts the passes taking the dependencies into account
         */
        void compile();

        void generateDot(const std::string& filename);

    private:

        void dfs(PassID passID, std::unordered_map<PassID , bool>& visited);

    private:
        PassMap m_PassMap;

        std::vector<Pass*> m_ExecutorList; // the passes in the order they should be executed
        std::unordered_map<PassID, PassDesc> m_PassDescs;
    };

}