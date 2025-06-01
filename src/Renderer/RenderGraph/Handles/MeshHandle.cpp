#include "MeshHandle.hpp"

namespace Syrius::Renderer {

     MeshHandle::MeshHandle(MeshID meshID, const ResourceView<Context> &ctx, const Mesh &mesh,
                           const ResourceView<ShaderModule> &vertexShader,
                           const ResourceView<VertexLayout> &vertexDesc):
    m_MeshID(meshID){
         VertexBufferDesc vbDesc;
         vbDesc.data = mesh.vertices.data();
         vbDesc.count = mesh.vertices.size();
         vbDesc.layout = vertexDesc;
         vbDesc.usage = SR_BUFFER_USAGE_STATIC;
         m_VertexBuffer = ctx->createVertexBuffer(vbDesc);

         IndexBufferDesc ibDesc;
         ibDesc.data = mesh.indices.data();
         ibDesc.count = mesh.indices.size();
         ibDesc.usage = SR_BUFFER_USAGE_STATIC;
         ibDesc.dataType = SR_UINT32;
         m_IndexBuffer = ctx->createIndexBuffer(ibDesc);

         VertexArrayDesc vaDesc;
         vaDesc.vertexBuffer = m_VertexBuffer;
         vaDesc.indexBuffer = m_IndexBuffer;
         vaDesc.vertexShader = vertexShader;
         vaDesc.drawMode = SR_DRAW_TRIANGLES;
         m_VertexArray = ctx->createVertexArray(vaDesc);

         SR_LOG_INFO("MeshHandle", "MeshID {} created with {} vertices and {} indices", m_MeshID, mesh.vertices.size(), mesh.indices.size());
    }

    MeshHandle::MeshHandle(MeshHandle &&other) noexcept:
    m_MeshID(other.m_MeshID),
    m_VertexBuffer(other.m_VertexBuffer),
    m_IndexBuffer(other.m_IndexBuffer),
    m_VertexArray(other.m_VertexArray),
    m_InstanceCount(other.m_InstanceCount),
    m_InstanceToTransform(other.m_InstanceToTransform) {

    }

    MeshHandle &MeshHandle::operator=(MeshHandle &&other) noexcept {
        if (this == &other){
            return *this;
        }
        m_MeshID = other.m_MeshID;
        m_VertexBuffer = other.m_VertexBuffer;
        m_IndexBuffer = other.m_IndexBuffer;
        m_VertexArray = other.m_VertexArray;
        m_InstanceToTransform = other.m_InstanceToTransform;
        m_InstanceCount = other.m_InstanceCount;
        return *this;
    }

    void MeshHandle::createInstance(InstanceID instanceID) {
        if (m_InstanceToTransform.getSize() >= SR_MAX_INSTANCES){
            SR_LOG_WARNING("MeshHandle", "Cannot create more instances than {}", SR_MAX_INSTANCES);
            return;
        }
        if (m_InstanceToTransform.has(instanceID)){
            SR_LOG_WARNING("MeshHandle", "InstanceID {} already exists for mesh {}", instanceID, m_MeshID);
            return;
        }
        m_InstanceToTransform.emplace(instanceID, InstanceData());
        m_InstanceCount++;
        SR_LOG_INFO("MeshHandle", "InstanceID {} created for mesh {}", instanceID, m_MeshID);

        SR_POSTCONDITION(m_InstanceToTransform.has(instanceID), "InstanceID {} does not exist for mesh {}", instanceID, m_MeshID);
    }

    void MeshHandle::setTransformation(InstanceID instanceID, const Transform &transform) {
        if (!m_InstanceToTransform.has(instanceID)){
            SR_LOG_WARNING("MeshHandle", "InstanceID {} does not exist for mesh {}", instanceID, m_MeshID);
            return;
        }

        m_InstanceToTransform[instanceID].modelMatrix = transform.model;
        m_InstanceToTransform[instanceID].inverseTranspose = glm::transpose(glm::inverse(transform.model));
    }

    void MeshHandle::removeInstance(InstanceID instanceID) {
        if (!m_InstanceToTransform.has(instanceID)){
            SR_LOG_WARNING("MeshHandle", "InstanceID {} does not exist for mesh {}", instanceID, m_MeshID);
            return;
        }
        m_InstanceToTransform.remove(instanceID);
        m_InstanceCount--;

        SR_POSTCONDITION(!m_InstanceToTransform.has(instanceID), "InstanceID {} still exists for mesh {}", instanceID, m_MeshID);
    }

    void MeshHandle::drawMesh(const ResourceView<Context> &ctx) const {
        ctx->drawInstanced(m_VertexArray, m_InstanceCount);
    }

}
