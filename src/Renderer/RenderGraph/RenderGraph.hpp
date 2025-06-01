#pragma once

#include "RenderGraphContainer.hpp"
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
         * @return false if compilation failed.
         */
        bool compile();

        void execute(RenderGraphContainer* graphData, const ResourceView<Context>& ctx) const;

        void generateDot() const;

    private:
        std::vector<RenderGraphNode> m_Nodes;
        std::vector<RenderGraphNode*> m_ExecuteList;
    };

}