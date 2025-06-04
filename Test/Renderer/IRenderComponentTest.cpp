#include "IRenderComponentTest.hpp"

#include <SyriusEngine/Renderer/BuiltinMeshes.hpp>
#include <SyriusEngine/Renderer/Material.hpp>

UP<SyriusWindow> IRenderComponentTest::m_Window = nullptr;
ResourceView<Context> IRenderComponentTest::m_Context;
bool IRenderComponentTest::m_SetupDone = false;

void IRenderComponentTest::SetUp() {
    if (!m_SetupDone) {
        WindowDesc wDesc;
        wDesc.width = 800;
        wDesc.height = 600;
        m_Window = createWindow(wDesc);

        ContextDesc cDesc;
        cDesc.api = SR_API_OPENGL;      // Most compatible
        m_Context = m_Window->createContext(cDesc);
        m_SetupDone = true;
    }

    m_WorkerPool = createSP<WorkerPool>();
    m_DispatcherManager = createSP<DispatcherManager>(m_WorkerPool);
}

void IRenderComponentTest::TearDown() {

}

MeshID IRenderComponentTest::createMesh() const {
    static MeshID meshID = 0;
    static Mesh rectangle = createRectangle();

    const MeshID newID = meshID++;
    const auto dispatcher = m_DispatcherManager->getDispatcher<MeshID, Mesh>();
    const auto dataPtr = createSP<Mesh>(rectangle);
    dispatcher->dispatchCreate(newID, dataPtr);
    return newID;
}

void IRenderComponentTest::createMesh(const MeshID meshID) const {
    static Mesh rectangle = createRectangle();
    const auto dispatcher = m_DispatcherManager->getDispatcher<MeshID, Mesh>();
    const auto dataPtr = createSP<Mesh>(rectangle);
    dispatcher->dispatchCreate(meshID, dataPtr);
}

void IRenderComponentTest::destroyMesh(const MeshID meshID) const {
    const auto dispatcher = m_DispatcherManager->getDispatcher<MeshID, Mesh>();
    dispatcher->dispatchDelete(meshID);
}

InstanceID IRenderComponentTest::createInstance(const MeshID meshID) const {
    static InstanceID instanceID = 0;

    const InstanceID newID = instanceID++;
    const auto dispatcher = m_DispatcherManager->getDispatcher<InstanceID, MeshID>();
    const auto dataPtr = createSP<MeshID>(meshID);
    dispatcher->dispatchCreate(newID, dataPtr);
    return newID;
}

void IRenderComponentTest::destroyInstance(const InstanceID instanceID) const {
    const auto dispatcher = m_DispatcherManager->getDispatcher<InstanceID, MeshID>();
    dispatcher->dispatchDelete(instanceID);
}

void IRenderComponentTest::setInstanceTransformation(const InstanceID instanceID, const Transform& transform) const {
    const auto dispatcher = m_DispatcherManager->getDispatcher<InstanceID, Transform>();
    const auto dataPtr = createSP<Transform>(transform);
    dispatcher->dispatchUpdate(instanceID, dataPtr);
}

MaterialID IRenderComponentTest::createMaterial() const {
    // 2x2 chessboard texture (black and purple)
    const std::vector<u8> albedo = {
        0, 0, 0, 255, 128, 0, 128, 255,
        128, 0, 128, 255, 0, 0, 0, 255
    };
    // 1x1 normal texture (normal pointing up)
    const std::vector<u8> normal = {
        0, 0, 255, 255, 0, 0, 255, 255,
        0, 0, 255, 255, 0, 0, 255, 255
    };
    // 1x1 MRAO texture (roughness 0.5)
    std::vector<u8> mrao = {
        128, 128, 128, 255, 128, 128, 128, 255,
        128, 128, 128, 255, 128, 128, 128, 255
    };
    ImageUI8Desc albedoDesc;
    albedoDesc.width = 2;
    albedoDesc.height = 2;
    albedoDesc.data = albedo.data();
    albedoDesc.format = SR_TEXTURE_RGBA_UI8;

    ImageUI8Desc normalDesc;
    normalDesc.width = 2;
    normalDesc.height = 2;
    normalDesc.data = normal.data();
    normalDesc.format = SR_TEXTURE_RGBA_UI8;

    ImageUI8Desc mraoDesc;
    mraoDesc.width = 2;
    mraoDesc.height = 2;
    mraoDesc.data = mrao.data();
    mraoDesc.format = SR_TEXTURE_RGBA_UI8;

    UP<Image> albedoImage = createImage(albedoDesc);
    UP<Image> normalImage = createImage(normalDesc);
    UP<Image> mraoImage = createImage(mraoDesc);

    auto material = std::make_shared<Material>(std::move(albedoImage), std::move(normalImage), std::move(mraoImage));

    static MaterialID materialID = 0;

    const MaterialID newID = materialID++;
    const auto dispatcher = m_DispatcherManager->getDispatcher<MaterialID, Material>();
    dispatcher->dispatchCreate(newID, material);
    return newID;
}

void IRenderComponentTest::destroyMaterial(const MaterialID materialID) const {
    const auto dispatcher = m_DispatcherManager->getDispatcher<MaterialID, Material>();
    dispatcher->dispatchDelete(materialID);
}

void IRenderComponentTest::setMeshMaterial(const MeshID meshID, const MaterialID materialID) const {
    const auto dispatcher = m_DispatcherManager->getDispatcher<MeshID, MaterialID>();
    const auto dataPtr = createSP<MaterialID>(materialID);
    dispatcher->dispatchUpdate(meshID, dataPtr);
}



