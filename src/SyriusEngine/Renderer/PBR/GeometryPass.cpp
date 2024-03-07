#include "GeometryPass.hpp"
#include "CameraDataPass.hpp"
#include "ProjectionPass.hpp"
#include "LFWRSamplerPass.hpp"

namespace Syrius{

    GeometryPass::GeometryPass(ResourceView<Context> &context, Resource<ShaderLibrary>& shaderLibrary, const GeometryPassDesc &gpDesc):
    RenderPass(context, shaderLibrary, "Geometry", PassIDGenerator<GeometryPass>::getID()),
    m_Slot(gpDesc.modelDataBufferSlot){
        addDependency<ProjectionPass>();
        addDependency<LFWRSamplerPass>();
        addDependency<CameraDataPass>();

        m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("TexCoords", SR_FLOAT32_2);

        TransformData transformData;
        ConstantBufferDesc cbDesc;
        cbDesc.name = gpDesc.modelDataBufferName;
        cbDesc.size = sizeof(TransformData);
        cbDesc.shaderStage = SR_SHADER_VERTEX;
        cbDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cbDesc.data = &transformData;
        m_ModelData = m_Context->createConstantBuffer(cbDesc);

        loadShader("GeometryPass");

        SR_POSTCONDITION(m_ModelData.get() != nullptr, "Invalid model data buffer (%p)",m_ModelData.get())
        SR_POSTCONDITION(m_Shader.get() != nullptr, "Invalid shader (%p)", m_Shader.get());
        SR_POSTCONDITION(m_VertexLayout->getStride() != 0, "Vertex Layout stride is %i", m_VertexLayout->getStride());
    }

    GeometryPass::~GeometryPass() {

    }

    void GeometryPass::execute(PassMap& passMap) {
        m_Context->beginRenderPass();

        m_ModelData->bind(m_Slot);
        m_Shader->bind();
        for (const auto& mesh: m_Meshes){
            m_ModelData->setData(&mesh.transformData);
            mesh.draw();
        }
        m_Context->endRenderPass();
    }

    MeshID GeometryPass::createMesh(const MeshDesc &mesh) {
        auto mid = generateID();
        m_Meshes.emplace(mid, m_Context, mesh, m_VertexShader, m_VertexLayout);

        SR_POSTCONDITION(mid != 0, "MeshID 0 is reserved for invalid mesh")
        return mid;
    }

    void GeometryPass::transformMesh(MeshID meshID, const glm::mat4 &transform) {
        SR_PRECONDITION(m_Meshes.has(meshID), "MeshID: %d does not exist", meshID)

        m_Meshes[meshID].setTransformation(transform);
    }

    void GeometryPass::removeMesh(MeshID meshID) {
        SR_PRECONDITION(m_Meshes.has(meshID), "MeshID: %d does not exist", meshID)

        m_Meshes.remove(meshID);

    }
}
