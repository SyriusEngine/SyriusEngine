#include "MeshHandle.hpp"

namespace Syrius{

    MeshHandle::MeshHandle(ResourceView<Context> &context, const MeshDesc &mesh, ResourceView<ShaderModule> &vertexShader,
                           ResourceView<VertexLayout> &vertexDesc):
                           m_Context(context),
                           materialID(0){


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
    }

    MeshHandle::MeshHandle(MeshHandle &&other) noexcept:
    m_Context(other.m_Context),
    m_VertexBuffer(other.m_VertexBuffer),
    m_IndexBuffer(other.m_IndexBuffer),
    m_VertexArray(other.m_VertexArray),
    materialID(other.materialID),
    transformData(other.transformData){

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
        return *this;
    }

    MeshHandle::~MeshHandle() {

    }

    void MeshHandle::setTransformation(const glm::mat4 &transform) {
        transformData.transform = transform;
        transformData.inverseTranspose = glm::transpose(glm::inverse(transform));
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
        m_Context->draw(m_VertexArray);
    }

}
