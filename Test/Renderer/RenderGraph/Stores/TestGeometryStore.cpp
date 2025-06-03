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

    void createMesh(const MeshID meshID) const {
        static Mesh rectangle = createRectangle();
        const auto dispatcher = m_DispatcherManager->getDispatcher<MeshID, Mesh>();
        const auto dataPtr = createSP<Mesh>(rectangle);
        dispatcher->dispatchCreate(meshID, dataPtr);
    }

    void destroyMesh(const MeshID meshID) const {
        const auto dispatcher = m_DispatcherManager->getDispatcher<MeshID, Mesh>();
        dispatcher->dispatchDelete(meshID);
    }

    InstanceID createInstance(const MeshID meshID) const {
        static InstanceID instanceID = 0;

        const InstanceID newID = instanceID++;
        const auto dispatcher = m_DispatcherManager->getDispatcher<InstanceID, MeshID>();
        const auto dataPtr = createSP<MeshID>(meshID);
        dispatcher->dispatchCreate(newID, dataPtr);
        return newID;
    }

    void destroyInstance(const InstanceID instanceID) const {
        const auto dispatcher = m_DispatcherManager->getDispatcher<InstanceID, MeshID>();
        dispatcher->dispatchDelete(instanceID);
    }

    void setInstanceTransformation(const InstanceID instanceID, const Transform& transform) const {
        const auto dispatcher = m_DispatcherManager->getDispatcher<InstanceID, Transform>();
        const auto dataPtr = createSP<Transform>(transform);
        dispatcher->dispatchUpdate(instanceID, dataPtr);
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

TEST_F(TestGeometryStore, CreateMeshDuplicateID) {
    m_Container->createData<GeometryStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto geometryStore = m_Container->getData<GeometryStore>();
    const auto& meshHandles = geometryStore->getMeshHandles();

    createMesh(1);
    createMesh(1); // Should fail

    EXPECT_EQ(meshHandles.getSize(), 1);
}

TEST_F(TestGeometryStore, DestroyMesh) {
    m_Container->createData<GeometryStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto geometryStore = m_Container->getData<GeometryStore>();
    const auto& meshHandles = geometryStore->getMeshHandles();

    auto m1 = createMesh();
    auto m2 = createMesh();
    auto m3 = createMesh();

    EXPECT_TRUE(meshHandles.has(m1));
    EXPECT_TRUE(meshHandles.has(m2));
    EXPECT_TRUE(meshHandles.has(m3));

    destroyMesh(m1);
    EXPECT_FALSE(meshHandles.has(m1));
    EXPECT_TRUE(meshHandles.has(m2));
    EXPECT_TRUE(meshHandles.has(m3));
    destroyMesh(m2);
    EXPECT_FALSE(meshHandles.has(m1));
    EXPECT_FALSE(meshHandles.has(m2));
    EXPECT_TRUE(meshHandles.has(m3));
    destroyMesh(m3);
    EXPECT_FALSE(meshHandles.has(m1));
    EXPECT_FALSE(meshHandles.has(m2));
    EXPECT_FALSE(meshHandles.has(m3));
}

TEST_F(TestGeometryStore, CreateInstance) {
    m_Container->createData<GeometryStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto geometryStore = m_Container->getData<GeometryStore>();
    const auto& instanceToMesh = geometryStore->get_m_InstanceToMeshID();
    const auto mesh = createMesh();
    const MeshHandle& handle = geometryStore->getMeshHandles().get(mesh);
    const auto& instanceToTransform = handle.get_m_InstanceToTransform();

    auto i1 = createInstance(mesh);
    auto i2 = createInstance(mesh);
    auto i3 = createInstance(mesh);

    EXPECT_TRUE(instanceToMesh.find(i1) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i1));
    EXPECT_TRUE(instanceToMesh.find(i2) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i2));
    EXPECT_TRUE(instanceToMesh.find(i3) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i3));
}

TEST_F(TestGeometryStore, DestroyInstance) {
    m_Container->createData<GeometryStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto geometryStore = m_Container->getData<GeometryStore>();
    const auto& instanceToMesh = geometryStore->get_m_InstanceToMeshID();
    const auto mesh = createMesh();
    const MeshHandle& handle = geometryStore->getMeshHandles().get(mesh);
    const auto& instanceToTransform = handle.get_m_InstanceToTransform();

    auto i1 = createInstance(mesh);
    auto i2 = createInstance(mesh);
    auto i3 = createInstance(mesh);

    EXPECT_TRUE(instanceToMesh.find(i1) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i1));
    EXPECT_TRUE(instanceToMesh.find(i2) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i2));
    EXPECT_TRUE(instanceToMesh.find(i3) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i3));

    destroyInstance(i1);
    EXPECT_TRUE(instanceToMesh.find(i1) == instanceToMesh.end());
    EXPECT_FALSE(instanceToTransform.has(i1));
    EXPECT_TRUE(instanceToMesh.find(i2) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i2));
    EXPECT_TRUE(instanceToMesh.find(i3) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i3));

    destroyInstance(i3);
    EXPECT_TRUE(instanceToMesh.find(i1) == instanceToMesh.end());
    EXPECT_FALSE(instanceToTransform.has(i1));
    EXPECT_TRUE(instanceToMesh.find(i2) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i2));
    EXPECT_TRUE(instanceToMesh.find(i3) == instanceToMesh.end());
    EXPECT_FALSE(instanceToTransform.has(i3));

    destroyInstance(i2);
    EXPECT_TRUE(instanceToMesh.find(i1) == instanceToMesh.end());
    EXPECT_FALSE(instanceToTransform.has(i1));
    EXPECT_TRUE(instanceToMesh.find(i2) == instanceToMesh.end());
    EXPECT_FALSE(instanceToTransform.has(i2));
    EXPECT_TRUE(instanceToMesh.find(i3) == instanceToMesh.end());
    EXPECT_FALSE(instanceToTransform.has(i3));
}

TEST_F(TestGeometryStore, DestroyMeshWithInstances) {
    m_Container->createData<GeometryStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto geometryStore = m_Container->getData<GeometryStore>();
    const auto& instanceToMesh = geometryStore->get_m_InstanceToMeshID();
    const auto mesh = createMesh();
    const MeshHandle& handle = geometryStore->getMeshHandles().get(mesh);
    const auto& instanceToTransform = handle.get_m_InstanceToTransform();

    auto i1 = createInstance(mesh);
    auto i2 = createInstance(mesh);
    auto i3 = createInstance(mesh);

    EXPECT_TRUE(instanceToMesh.find(i1) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i1));
    EXPECT_TRUE(instanceToMesh.find(i2) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i2));
    EXPECT_TRUE(instanceToMesh.find(i3) != instanceToMesh.end());
    EXPECT_TRUE(instanceToTransform.has(i3));

    destroyMesh(mesh);
    EXPECT_FALSE(geometryStore->getMeshHandles().has(mesh));
    EXPECT_TRUE(instanceToMesh.find(i1) == instanceToMesh.end());
    EXPECT_TRUE(instanceToMesh.find(i2) == instanceToMesh.end());
    EXPECT_TRUE(instanceToMesh.find(i3) == instanceToMesh.end());
}

TEST_F(TestGeometryStore, SetInstanceTransformation) {
    m_Container->createData<GeometryStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto geometryStore = m_Container->getData<GeometryStore>();
    const auto& instanceToMesh = geometryStore->get_m_InstanceToMeshID();
    const auto mesh = createMesh();
    const MeshHandle& handle = geometryStore->getMeshHandles().get(mesh);
    const auto& instanceToTransform = handle.get_m_InstanceToTransform();

    auto i1 = createInstance(mesh);
    Transform t1;
    t1.model = glm::mat4(4.0f);

    setInstanceTransformation(i1, t1);
    EXPECT_NEAR(instanceToTransform.get(i1).modelMatrix[0][0], t1.model[0][0], 1e-5);
}
