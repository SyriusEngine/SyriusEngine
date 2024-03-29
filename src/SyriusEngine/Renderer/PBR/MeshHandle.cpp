#include "MeshHandle.hpp"

namespace Syrius{

    MeshHandle::MeshHandle(ResourceView<Context> &context, const MeshDesc &mesh, ResourceView<ShaderModule> &vertexShader,
    ResourceView<VertexLayout> &vertexDesc):
    m_Context(context),
    materialID(0),
    m_InstanceToIndex(),
    transformData(SR_MAX_INSTANCES){
        VertexBufferDesc vbDesc;
        vbDesc.data = mesh.vertices.data();
        vbDesc.count = mesh.vertices.size();
        vbDesc.layout = vertexDesc;
        vbDesc.usage = SR_BUFFER_USAGE_STATIC;
        m_VertexBuffer = context->createVertexBuffer(vbDesc);

        IndexBufferDesc ibDesc;
        ibDesc.data = mesh.indices.data();
        ibDesc.count = mesh.indices.size();
        ibDesc.usage = SR_BUFFER_USAGE_STATIC;
        ibDesc.dataType = SR_UINT32;
        m_IndexBuffer = context->createIndexBuffer(ibDesc);

        VertexArrayDesc vaDesc;
        vaDesc.vertexBuffer = m_VertexBuffer;
        vaDesc.indexBuffer = m_IndexBuffer;
        vaDesc.vertexShader = vertexShader;
        vaDesc.drawMode = SR_DRAW_TRIANGLES;
        m_VertexArray = context->createVertexArray(vaDesc);

        for (uint32 i = 0; i < SR_MAX_INSTANCES; i++){
            transformData[i].transform = glm::mat4(1.0f);
            transformData[i].inverseTranspose = glm::mat4(1.0f);
        }
    }

    MeshHandle::MeshHandle(MeshHandle &&other) noexcept:
    m_Context(other.m_Context),
    m_VertexBuffer(other.m_VertexBuffer),
    m_IndexBuffer(other.m_IndexBuffer),
    m_VertexArray(other.m_VertexArray),
    materialID(other.materialID),
    transformData(other.transformData),
    m_InstanceToIndex(other.m_InstanceToIndex){
    }

    MeshHandle &MeshHandle::operator=(MeshHandle &&other) noexcept {
        if (this == &other){
            return *this;
        }
        m_Context = other.m_Context;
        m_VertexBuffer = other.m_VertexBuffer;
        m_IndexBuffer = other.m_IndexBuffer;
        m_VertexArray = other.m_VertexArray;
        materialID = other.materialID;
        transformData = other.transformData;
        m_InstanceToIndex = other.m_InstanceToIndex;
        return *this;
    }

    MeshHandle::~MeshHandle() {

    }

    MeshID MeshHandle::createInstance() {
        SR_PRECONDITION(m_InstanceToIndex.size() < SR_MAX_INSTANCES, "Cannot create more instances than %d", SR_MAX_INSTANCES);

        MeshID iid = generateID();
        auto index = m_InstanceToIndex.size();
        m_InstanceToIndex.insert({iid, index});
        transformData[index].transform = glm::mat4(1.0f);
        transformData[index].inverseTranspose = glm::mat4(1.0f);
        return iid;
    }

    void MeshHandle::setTransformation(MeshID meshID, const glm::mat4 &transform) {
        SR_PRECONDITION(m_InstanceToIndex.find(meshID) != m_InstanceToIndex.end(), "Instance %d does not exist", meshID);

        auto index = m_InstanceToIndex[meshID];
        transformData[index].transform = transform;
        transformData[index].inverseTranspose = glm::transpose(glm::inverse(transform));

    }

    void MeshHandle::removeInstance(MeshID meshID) {
        SR_PRECONDITION(m_InstanceToIndex.find(meshID) != m_InstanceToIndex.end(), "Instance %d does not exist", meshID);

        auto lastDataIndex = transformData.size() - 1;
        auto currentDataIndex = m_InstanceToIndex[meshID];
        MeshID lastElementKey;
        for (const auto& lastKey : m_InstanceToIndex){
            if (lastDataIndex == lastKey.second){
                lastElementKey = lastKey.first;
                break;
            }
        }
        // switch last and current element from place
        transformData[currentDataIndex] = std::move(transformData[lastDataIndex]);
        transformData.pop_back();
        m_InstanceToIndex.erase(meshID);
        // update index
        m_InstanceToIndex[lastElementKey] = currentDataIndex;
    }

    void MeshHandle::draw() const {
        /*
         * Vertex and Index buffer binding is only added for RenderDoc.
         * RenderDoc does not show the mesh otherwise.
         * The underlying implementation of the VAO uses DSA which does not require explicit vertex and index buffer
         * since this is handled by the Vertex Array Object.
         */
#if defined(SR_DEBUG)
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
#endif
        m_Context->drawInstanced(m_VertexArray, m_InstanceToIndex.size());
    }

}
