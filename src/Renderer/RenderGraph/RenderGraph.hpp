#pragma once

#include "RenderGraphData.hpp"
#include "RenderGraphNode.hpp"

namespace Syrius::Renderer {

    class RenderGraph {
    public:
        RenderGraph() = default;

        ~RenderGraph() = default;

        void addNode(RenderGraphNode& node);

        /**
         * @brief Checks if every added node its dependencies are added as well
         * @return false if a node has missing dependencies.
         */
        bool validate() const;

        /**
         * @brief Topologically sorts the render graph and creates the executor list
         */
        void compile();

        void execute(RenderGraphData& graphData, const ResourceView<Context>& ctx) const;

    private:
        std::vector<RenderGraphNode> m_Nodes;
        std::vector<RenderGraphNode*> m_ExecuteList;
    };

}