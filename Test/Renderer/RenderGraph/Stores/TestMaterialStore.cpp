#include "../../../../src/Renderer/RenderGraph/RenderGraphContainer.hpp"
#include "../../../../src/Renderer/RenderGraph/Stores/MaterialStore.hpp"
#include "../../IRenderComponentTest.hpp"

using namespace Syrius;
using namespace Syrius::Renderer;

class TestMaterialStore: public IRenderComponentTest {
protected:
    void SetUp() override {
        IRenderComponentTest::SetUp();
        m_Container = createUP<RenderGraphContainer>();
    }

protected:
    UP<RenderGraphContainer> m_Container;
};

TEST_F(TestMaterialStore, CreateMesh) {
    m_Container->createData<MaterialStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto materialStore = m_Container->getData<MaterialStore>();
    const auto& meshToMaterial = materialStore->get_m_MeshToMaterial();

    auto m1 = createMesh();
    auto m2 = createMesh();
    auto m3 = createMesh();

    EXPECT_TRUE(meshToMaterial.find(m1) != meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m2) != meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m3) != meshToMaterial.end());
}

TEST_F(TestMaterialStore, DestroyMesh) {
    m_Container->createData<MaterialStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto materialStore = m_Container->getData<MaterialStore>();
    const auto& meshToMaterial = materialStore->get_m_MeshToMaterial();

    auto m1 = createMesh();
    auto m2 = createMesh();
    auto m3 = createMesh();

    EXPECT_TRUE(meshToMaterial.find(m1) != meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m2) != meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m3) != meshToMaterial.end());

    destroyMesh(m1);
    EXPECT_TRUE(meshToMaterial.find(m1) == meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m2) != meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m3) != meshToMaterial.end());

    destroyMesh(m3);
    EXPECT_TRUE(meshToMaterial.find(m1) == meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m2) != meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m3) == meshToMaterial.end());

    destroyMesh(m2);
    EXPECT_TRUE(meshToMaterial.find(m1) == meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m2) == meshToMaterial.end());
    EXPECT_TRUE(meshToMaterial.find(m3) == meshToMaterial.end());
}

TEST_F(TestMaterialStore, CreateMaterial) {
    m_Container->createData<MaterialStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto materialStore = m_Container->getData<MaterialStore>();
    const auto& materials = materialStore->get_m_Materials();

    auto m1 = createMaterial();
    auto m2 = createMaterial();
    auto m3 = createMaterial();

    EXPECT_TRUE(materials.has(m1));
    EXPECT_TRUE(materials.has(m2));
    EXPECT_TRUE(materials.has(m3));
}

TEST_F(TestMaterialStore, DestroyMaterial) {
    m_Container->createData<MaterialStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto materialStore = m_Container->getData<MaterialStore>();
    const auto& materials = materialStore->get_m_Materials();

    auto m1 = createMaterial();
    auto m2 = createMaterial();
    auto m3 = createMaterial();

    EXPECT_TRUE(materials.has(m1));
    EXPECT_TRUE(materials.has(m2));
    EXPECT_TRUE(materials.has(m3));

    destroyMaterial(m2);
    EXPECT_TRUE(materials.has(m1));
    EXPECT_FALSE(materials.has(m2));
    EXPECT_TRUE(materials.has(m3));

    destroyMaterial(m3);
    EXPECT_TRUE(materials.has(m1));
    EXPECT_FALSE(materials.has(m2));
    EXPECT_FALSE(materials.has(m3));

    destroyMaterial(m1);
    EXPECT_FALSE(materials.has(m1));
    EXPECT_FALSE(materials.has(m2));
    EXPECT_FALSE(materials.has(m3));
}

TEST_F(TestMaterialStore, SetMeshMaterial) {
    m_Container->createData<MaterialStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto materialStore = m_Container->getData<MaterialStore>();
    const auto& materials = materialStore->get_m_Materials();
    const auto& meshToMaterial = materialStore->get_m_MeshToMaterial();

    auto material = createMaterial();
    auto m1 = createMesh();
    auto m2 = createMesh();
    auto m3 = createMesh();

    setMeshMaterial(m1, material);
    setMeshMaterial(m2, material);
    setMeshMaterial(m3, material);

    EXPECT_TRUE(materials.has(material));
    EXPECT_EQ(meshToMaterial.at(m2), material);
    EXPECT_EQ(meshToMaterial.at(m3), material);
    EXPECT_EQ(meshToMaterial.at(m1), material);
}

TEST_F(TestMaterialStore, DestroyMaterialStillInUse) {
    /*
     * When a material is destroyed while still in use, the meshes should get reset to use the default material
     */
    m_Container->createData<MaterialStore>(m_Context, m_Container.get(), m_DispatcherManager);
    const auto materialStore = m_Container->getData<MaterialStore>();
    const auto& materials = materialStore->get_m_Materials();
    const auto& meshToMaterial = materialStore->get_m_MeshToMaterial();

    auto material = createMaterial();
    auto m1 = createMesh();
    auto m2 = createMesh();

    setMeshMaterial(m1, material);
    setMeshMaterial(m2, material);

    EXPECT_TRUE(materials.has(material));
    EXPECT_EQ(meshToMaterial.at(m1), material);
    EXPECT_EQ(meshToMaterial.at(m2), material);

    destroyMaterial(material);
    EXPECT_FALSE(materials.has(material));
    EXPECT_EQ(meshToMaterial.at(m1), SR_DEFAULT_MATERIAL);
    EXPECT_EQ(meshToMaterial.at(m2), SR_DEFAULT_MATERIAL);
}
