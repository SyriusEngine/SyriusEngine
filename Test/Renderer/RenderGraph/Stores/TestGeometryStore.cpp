#include "../../../../src/Renderer/RenderGraph/RenderGraphContainer.hpp"
#include "../../../../src/Renderer/RenderGraph/Stores/GeometryStore.hpp"
#include "../../../../src/Renderer/RenderGraph/Stores/ShaderStore.hpp"
#include "../../IRenderComponentTest.hpp"
#include "SyriusEngine/Renderer/BuiltinMeshes.hpp"

using namespace Syrius;
using namespace Syrius::Renderer;

class TestGeometryStore: public IRenderComponentTest {
protected:
    void SetUp() override {
        IRenderComponentTest::SetUp();
        m_Container = createUP<RenderGraphContainer>();
        const fs::path shaderPath = fs::current_path() / "Resources" / "Shaders";
        m_Container->createData<ShaderStore>(m_Context, m_Container.get(), shaderPath);
    }

    MeshID createMesh() const {
        static MeshID meshID = 0;
        static Mesh rectangle = createRectangle();

        const MeshID newID = meshID++;
        const auto dispatcher = m_DispatcherManager->getDispatcher<MeshID, Mesh>();
        const auto dataPtr = createSP<Mesh>(rectangle);
        dispatcher->dispatchCreate(newID, dataPtr);
        return newID;
    }

protected:
    UP<RenderGraphContainer> m_Container;
};

TEST_F(TestGeometryStore, CreateMesh) {
    m_Container->createData<GeometryStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto geometryStore = m_Container->getData<GeometryStore>();
    const auto& meshHandles = geometryStore->getMeshHandles();

    auto m1 = createMesh();
    auto m2 = createMesh();
    auto m3 = createMesh();

    EXPECT_TRUE(meshHandles.has(m1));
    EXPECT_TRUE(meshHandles.has(m2));
    EXPECT_TRUE(meshHandles.has(m3));

}
