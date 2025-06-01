#include "GeometryStore.hpp"
#include "../RenderGraphDefs.hpp"
#include "../RenderGraphContainer.hpp"
#include "ShaderStore.hpp"

namespace Syrius::Renderer {

    GeometryStore::GeometryStore(const ResourceView<Context>& ctx, RenderGraphContainer* container, const SP<DispatcherManager>& dispatcherManager):
    IRenderGraphData(ctx, container) {
        // Setup Vertex Layout
        m_VertexLayout = ctx->createVertexLayout();
        m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("TexCoords", SR_FLOAT32_2);

        // Attach dispatching logic
        const auto meshDispatcher = dispatcherManager->getDispatcher<MeshID, Mesh>();
        meshDispatcher->registerCreate([this](const MeshID meshID, const SP<Mesh>& mesh) {
            createMesh(meshID, mesh);
        }, SR_WORKER_RENDERER);
        meshDispatcher->registerDelete([this](const MeshID meshID) {
            destroyMesh(meshID);
        }, SR_WORKER_RENDERER);

        const auto instanceDispatcher = dispatcherManager->getDispatcher<InstanceID, MeshID>();
        instanceDispatcher->registerCreate([this](const InstanceID instanceID, const SP<MeshID> &meshID) {
            createInstance(instanceID, meshID);
        }, SR_WORKER_RENDERER);
        instanceDispatcher->registerDelete([&](const InstanceID instanceID) {
            destroyInstance(instanceID);
        }, SR_WORKER_RENDERER);

        SR_LOG_INFO("GeometryStore", "GeometryStore Created!");
    }

    void GeometryStore::createMesh(MeshID meshID, const SP<Mesh> &mesh) {
        if (m_MeshHandles.has(meshID)) {
            SR_LOG_WARNING("GeometryStore", "Mesh {} already created", meshID);
            return;
        }
        const auto shaderStore = m_Container->getData<ShaderStore>();
        const ShaderProgram& geometryPassShader = shaderStore->getShader(s_GEOMETRY_PASS_SHADER);
        m_MeshHandles.emplace(meshID, meshID, m_Ctx, *mesh, geometryPassShader.vertexShader, m_VertexLayout);
    }

    void GeometryStore::createInstance(InstanceID instanceID, const SP<MeshID>& meshID) {
        if (!m_MeshHandles.has(*meshID)) {
            SR_LOG_WARNING("GeometryStore", "Mesh {} does not exist!", *meshID);
            return;
        }
        MeshHandle& meshHandle = m_MeshHandles.get(*meshID);
        meshHandle.createInstance(instanceID);
        m_InstanceToMeshID.emplace(instanceID, *meshID);
    }

    void GeometryStore::destroyMesh(MeshID meshID) {
        if (!m_MeshHandles.has(meshID)) {
            SR_LOG_WARNING("GeometryStore", "Mesh {} does not exist!", meshID);
            return;
        }
        m_MeshHandles.remove(meshID);
        auto it = m_InstanceToMeshID.begin();
        while (it != m_InstanceToMeshID.end()) {
            if (it->second == meshID) {
                it = m_InstanceToMeshID.erase(it);
            } else {
                ++it;
            }
        }
    }

    void GeometryStore::destroyInstance(InstanceID instanceID) {
        if (m_InstanceToMeshID.find(instanceID) == m_InstanceToMeshID.end()) {
            SR_LOG_WARNING("GeometryStore", "Instance {} does not exist!", instanceID);
            return;
        }
        MeshID meshID = m_InstanceToMeshID.at(instanceID);
        if (!m_MeshHandles.has(meshID)) {
            SR_LOG_WARNING("GeometryStore", "Mesh {} does not exist!", meshID);
            return;
        }
        MeshHandle& meshHandle = m_MeshHandles.get(meshID);
        meshHandle.removeInstance(instanceID);
    }

}