#include <gtest/gtest.h>
#include "../../../src/Renderer/RenderGraph/Handles/MeshHandle.hpp"

using namespace Syrius;

class TestMeshHandle: public ::testing::Test {
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
};
