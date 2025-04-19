#include <gtest/gtest.h>
#include "../../../src/Renderer/RenderGraph/RenderGraph.hpp"

using namespace Syrius;
using namespace Syrius::Renderer;

class TestRenderGraph : public ::testing::Test {
protected:
    void SetUp() override {
        WindowDesc wDesc;
        m_Window = createWindow(wDesc);
        ContextDesc cDesc;
        m_Context = m_Window->createContext(cDesc);
    }

    void TearDown() override {
    }

protected:
    UP<SyriusWindow> m_Window;
    ResourceView<Context> m_Context;
    RenderGraphData m_GraphData;
};

TEST_F(TestRenderGraph, ValidateGraph) {
    RenderGraph graph;

    RenderGraphNode node1 = {{SR_NODE_TRANSFORM_DATA, SR_NODE_CAMERA_DATA, SR_NODE_PROJECTION_DATA}, {SR_NODE_GEOMETRY_DATA}};
    RenderGraphNode node2 = {{SR_NODE_GEOMETRY_DATA}, {SR_NODE_LIGHT_DATA}};
    graph.addNode(node1);
    graph.addNode(node2);

    // not all types are added, so the graph is not valid!
    EXPECT_FALSE(graph.validate());

    RenderGraphNode node3 = {{}, {SR_NODE_TRANSFORM_DATA}};
    graph.addNode(node3);
    RenderGraphNode node4 = {{}, {SR_NODE_CAMERA_DATA}};
    graph.addNode(node4);
    RenderGraphNode node5 = {{}, {SR_NODE_PROJECTION_DATA}};
    graph.addNode(node5);

    // Now all types are added, thus the graph should be valid
    EXPECT_TRUE(graph.validate());
}

TEST_F(TestRenderGraph, SmallGraph) {
    RenderGraph graph;

    std::vector<u32> order;

    RenderGraphNode node1 = {
        {SR_NODE_DRAW_GEOMETRY},
        {SR_NODE_DRAW_LIGHTS},
        [&order](const ResourceView<Context>& ctx, RenderGraphData& graphData) {
            order.push_back(2);
        }
    };
    graph.addNode(node1);
    RenderGraphNode node2 = {
        {SR_NODE_PROJECTION_DATA},
        {SR_NODE_DRAW_GEOMETRY},
        [&order](const ResourceView<Context>& ctx, RenderGraphData& graphData) {
            order.push_back(1);
        }
    };
    graph.addNode(node2);
    RenderGraphNode node3 = {
        {},
        {SR_NODE_PROJECTION_DATA},
        [&order](const ResourceView<Context>& ctx, RenderGraphData& graphData) {
            order.push_back(0);
        }
    };
    graph.addNode(node3);

    graph.compile();
    graph.execute(m_GraphData, m_Context);
    ASSERT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 0);
    EXPECT_EQ(order[1], 1);
    EXPECT_EQ(order[2], 2);

}