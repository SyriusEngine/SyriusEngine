#include "RenderGraph.hpp"

namespace Syrius{

    RenderGraph::RenderGraph(){

    }

    RenderGraph::~RenderGraph() {

    }

    void RenderGraph::execute() {
        for (const auto& p : m_ExecutorList){
            p->execute(m_PassMap);
        }
    }

    void RenderGraph::validate() {
        SR_PRECONDITION(m_PassMap.size() > 0, "No passes inside the render graph");

        for (const auto& [passID, pass] : m_PassMap){
            for (const auto& dependency : pass->getDependencies()){
                SR_ASSERT(m_PassDescs.find(dependency) != m_PassDescs.end(), "Pass (%s) has a dependency that is not inside the graph: %zul",
                          pass->getName().c_str(), dependency);
            }
        }

    }

    void RenderGraph::compile() {
        m_ExecutorList.clear();

        std::unordered_map<PassID, bool> visited;
        for (const auto& passDesc : m_PassDescs){
            visited.insert({passDesc.first, false});
        }

        for (const auto& passDesc : m_PassDescs){
            if (!visited[passDesc.first]){
                dfs(passDesc.first, visited);
            }
        }
    }

    void RenderGraph::dfs(PassID passID, std::unordered_map<PassID , bool> &visited) {
        visited[passID] = true;

        for (const auto& dependency : m_PassDescs[passID].dependencies){
            if (!visited[dependency]){
                dfs(dependency, visited);
            }
        }

        m_ExecutorList.push_back(m_PassDescs[passID].pAddress);
    }

    void RenderGraph::generateDot(const std::string &filename) {
        std::ofstream file(filename);
        file << "digraph G {" << std::endl;
        file << "\trankdir=LR;" << std::endl;
        for (const auto& [passID, pass] : m_PassMap){
            file << "\t" << passID<< " [label=\"" << pass->getName() << "\"];" << std::endl;
            for (const auto& dependency : pass->getDependencies()){
                file << "\t" << dependency << "->" << passID  << ";" << std::endl;
            }
        }
        file << "}" << std::endl;
        file.close();
    }

}
