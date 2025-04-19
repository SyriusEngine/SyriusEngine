#include <gtest/gtest.h>
#include <SyriusEngine/Layer.hpp>
#include "../src/LayerStack.hpp"

using namespace Syrius;

class TestLayerStackLayer: public ILayer {
public:
    TestLayerStackLayer() = default;
    ~TestLayerStackLayer() override = default;

    void onAttach() override {
        attached = true;
    }

    void onDetach() override {
        detached = true;
    }

    void onUpdate(Duration deltaTime) override {
        updated = true;
    }

    bool onEvent(const Event &event) override {
        receivedEvent = true;
        if (event.type == SR_EVENT_WINDOW_CLOSED) {
            return false;
        }
        return true;
    }

public:
    bool attached = false;
    bool detached = false;
    bool updated = false;
    bool receivedEvent = false;
};

class TestLayerStack: public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }
};


TEST_F(TestLayerStack, LayerStack) {
    LayerStack stack;

    EXPECT_EQ(stack.getLayerCount(), 0);

    const auto testLayer = createSP<TestLayerStackLayer>();
    LayerID layerID = testLayer->getID();
    stack.pushLayer(testLayer);

    EXPECT_TRUE(testLayer->attached);
    EXPECT_EQ(stack.getLayerCount(), 1);

    stack.onUpdate(0.1s);
    EXPECT_TRUE(testLayer->updated);

    const WindowClosedEvent event;
    stack.onEvent(event);
    EXPECT_TRUE(testLayer->receivedEvent);

    stack.popLayer(layerID);
    EXPECT_TRUE(testLayer->detached);
    EXPECT_EQ(stack.getLayerCount(), 0);
}

TEST_F(TestLayerStack, LimitEventTravel) {
    LayerStack stack;
    const auto t1 = createSP<TestLayerStackLayer>();
    const auto t2 = createSP<TestLayerStackLayer>();
    stack.pushLayer(t1);
    stack.pushLayer(t2);

    EXPECT_FALSE(t1->receivedEvent);
    EXPECT_FALSE(t2->receivedEvent);

    // If this event is dispatched, the first pushed layer should NOT receive it
    // since the second layer marks this event as fully processed
    const WindowClosedEvent event;
    stack.onEvent(event);

    EXPECT_FALSE(t1->receivedEvent);
    EXPECT_TRUE(t2->receivedEvent);
}

TEST_F(TestLayerStack, LayerStackDeletion) {
    LayerStack stack;
    const auto t1 = createSP<TestLayerStackLayer>();
    const auto t2 = createSP<TestLayerStackLayer>();
    const auto t3 = createSP<TestLayerStackLayer>();
    stack.pushLayer(t1);
    stack.pushLayer(t2);
    stack.pushLayer(t3);

    EXPECT_EQ(stack.getLayerCount(), 3);
    EXPECT_TRUE(stack.hasLayer(t1->getID()));
    EXPECT_TRUE(stack.hasLayer(t2->getID()));
    EXPECT_TRUE(stack.hasLayer(t3->getID()));

    stack.popLayer(t2->getID());
    EXPECT_EQ(stack.getLayerCount(), 2);
    EXPECT_TRUE(stack.hasLayer(t1->getID()));
    EXPECT_FALSE(stack.hasLayer(t2->getID()));
    EXPECT_TRUE(stack.hasLayer(t3->getID()));

    stack.popLayer(t1->getID());
    EXPECT_EQ(stack.getLayerCount(), 1);
    EXPECT_FALSE(stack.hasLayer(t1->getID()));
    EXPECT_FALSE(stack.hasLayer(t2->getID()));
    EXPECT_TRUE(stack.hasLayer(t3->getID()));

    stack.popLayer(t3->getID());
    EXPECT_EQ(stack.getLayerCount(), 0);
    EXPECT_FALSE(stack.hasLayer(t1->getID()));
    EXPECT_FALSE(stack.hasLayer(t2->getID()));
    EXPECT_FALSE(stack.hasLayer(t3->getID()));
}
