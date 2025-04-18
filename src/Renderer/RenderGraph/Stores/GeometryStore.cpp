#include "GeometryStore.hpp"
#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    GeometryStore::GeometryStore(UP<ShaderStore>& shaderStore, const ResourceView<Context>& ctx):
    m_ShaderStore(shaderStore) {
        m_VertexLayout = ctx->createVertexLayout();
        m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("TexCoords", SR_FLOAT32_2);

        SR_LOG_INFO("GeometryStore", "GeometryStore Created!");
    }

    void GeometryStore::createMesh(MeshID meshID, const Mesh &mesh, const ResourceView<Context> &ctx) {
        if (m_MeshHandles.has(meshID)) {
            SR_LOG_WARNING("GeometryStore", "Mesh {} already created", meshID);
            return;
        }
        const ShaderProgram& geometryPassShader = m_ShaderStore->getShader(s_GEOMETRY_PASS_SHADER, ctx);
        m_MeshHandles.emplace(meshID, meshID, ctx, mesh, geometryPassShader.vertexShader, m_VertexLayout);
    }

    void GeometryStore::createInstance(InstanceID instanceID, MeshID meshID, const ResourceView<Context> &ctx) {
        if (!m_MeshHandles.has(meshID)) {
            SR_LOG_WARNING("GeometryStore", "Mesh {} does not exist!", meshID);
            return;
        }
        MeshHandle& meshHandle = m_MeshHandles.get(meshID);
        meshHandle.createInstance(instanceID);
        m_InstanceToMeshID.emplace(instanceID, meshID);
    }

    void GeometryStore::destroyMesh(MeshID meshID, const ResourceView<Context> &ctx) {
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