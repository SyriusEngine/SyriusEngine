#include "RenderGraph.hpp"
#include <unordered_set>
#include <queue>
#include <fstream>

namespace Syrius::Renderer {

    void RenderGraph::addNode(RenderGraphNode &node) {
        m_Nodes.push_back(node);
    }

    bool RenderGraph::validate() {
        // Build a set of all the known types
        std::unordered_set<SR_RENDER_NODE> knownNodes;
        for (const auto& node : m_Nodes) {
            for (const auto providedType: node.provides) {
                knownNodes.insert(providedType);
            }
        }

        // Check if all needed types are provided by any node
        for (const auto& node : m_Nodes) {
            for (const auto& neededType: node.needs) {
                if (knownNodes.find(neededType) == knownNodes.end()) {
                    SR_LOG_WARNING("RenderGraph", "Node {} needs type {} but it is not provided by any node", node.id, renderNodeToString(neededType));
                    return false;
                }
            }
        }
        return true;
    }

    void RenderGraph::compile() {
        if (!validate()) {
            SR_LOG_WARNING("RenderGraph", "Unable to compile the render graph, it is not valid");
        }
        // Build some lookup maps
        std::unordered_map<NodeID, RenderGraphNode*> nodeMap;
        std::unordered_map<NodeID, u32> inDegree;
        std::unordered_map<NodeID, std::vector<NodeID>> adjacencyList;
        for (auto& node : m_Nodes) {
            nodeMap[node.id] = &node;
            inDegree[node.id] = 0;
        }

        // Count the in-degrees and build the adjacency lists
        for (const auto& node : m_Nodes) {
            for (const auto& dependency : node.needs) {
                inDegree[dependency]++;
                adjacencyList[dependency].push_back(node.id);
            }
        }

        // We start our topological sort with the nodes that have no dependencies
        std::queue<NodeID> q;
        for (const auto& [nodeID, in]: inDegree) {
            if (in == 0) {
                q.push(nodeID);
            }
        }

        // Perform the topological sort
        std::vector<RenderGraphNode*> sorted;
        while (!q.empty()) {
            NodeID current = q.front();
            q.pop();
            sorted.push_back(nodeMap[current]);

            for (const NodeID neighbor : adjacencyList[current]) {
                inDegree[neighbor]--;
                if (inDegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
        // Check if we have a cycle
        if (sorted.size() != m_Nodes.size()) {
            SR_LOG_ERROR("RenderGraph", "Graph has a cycle, cannot execute");
            return;
        }
        // TODO: The executor list should not exist, we should swap the nodes in the m_Nodes list around
        m_ExecuteList = sorted;
    }

    void RenderGraph::execute(RenderGraphData &graphData, const ResourceView<Context> &ctx) const {
        for (const auto* node: m_ExecuteList) {
            node->executor(ctx, graphData);
        }
    }

}
