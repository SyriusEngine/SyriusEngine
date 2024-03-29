#include "GeometryPass.hpp"
#include "CameraDataPass.hpp"
#include "ProjectionPass.hpp"
#include "LFWRSamplerPass.hpp"
#include "GBufferPass.hpp"

namespace Syrius{

    GeometryPass::GeometryPass(ResourceView<Context> &context, Resource<ShaderLibrary>& shaderLibrary, const GeometryPassDesc &gpDesc):
    RenderPass(context, shaderLibrary, "Geometry", PassIDGenerator<GeometryPass>::getID()),
    m_Slot(gpDesc.modelDataBufferSlot),
    m_MaterialStartSlot(gpDesc.materialStartingSlot){
        addDependency<ProjectionPass>();
        addDependency<LFWRSamplerPass>();
        addDependency<CameraDataPass>();
        addDependency<GBufferPass>();

        m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
        m_VertexLayout->addAttribute("TexCoords", SR_FLOAT32_2);

        TransformData transformData[SR_MAX_INSTANCES];
        ConstantBufferDesc cbDesc;
        cbDesc.name = gpDesc.modelDataBufferName;
        cbDesc.size = sizeof(TransformData);
        cbDesc.shaderStage = SR_SHADER_VERTEX;
        cbDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cbDesc.data = &transformData;
        m_ModelData = m_Context->createConstantBuffer(cbDesc);

        loadShader("GeometryPass");

        createDefaultMaterial();

        SR_POSTCONDITION(m_ModelData.get() != nullptr, "Invalid model data buffer (%p)",m_ModelData.get())
        SR_POSTCONDITION(m_Shader.get() != nullptr, "Invalid shader (%p)", m_Shader.get());
        SR_POSTCONDITION(m_VertexLayout->getStride() != 0, "Vertex Layout stride is %i", m_VertexLayout->getStride());
        SR_POSTCONDITION(m_Materials.has(0), "Default material (%d) does not exist", 0)
    }

    GeometryPass::~GeometryPass() {

    }

    void GeometryPass::execute(PassMap& passMap) {
        auto gbp = passMap.getPass<GBufferPass>();
        gbp->beginRenderPass();

        m_ModelData->bind(m_Slot);
        m_Shader->bind();
        for (const auto& mesh: m_Meshes){
            m_ModelData->setData(mesh.transformData.data());
            m_Materials[mesh.materialID].bind();
            mesh.draw();
        }
        gbp->endRenderPass();
    }

    MeshID GeometryPass::createMesh(const MeshDesc &mesh) {
        auto mid = generateID();
        m_Meshes.emplace(mid, m_Context, mesh, m_VertexShader, m_VertexLayout);

        auto iid = m_Meshes[mid].createInstance();
        m_MeshInstances.emplace(iid, mid);

        SR_POSTCONDITION(mid != 0, "MeshID 0 is reserved for invalid mesh")
        return iid;
    }

    void GeometryPass::transformMesh(MeshID meshID, const glm::mat4 &transform) {
        MeshDataID meshDataID = m_MeshInstances[meshID];

        m_Meshes[meshDataID].setTransformation(meshID, transform);
    }

    void GeometryPass::removeMesh(MeshID meshID) {
        MeshDataID meshDataID = m_MeshInstances[meshID];
        m_MeshInstances.erase(meshID);

        auto& meshHandle = m_Meshes[meshDataID];
        meshHandle.removeInstance(meshID);

        if (meshHandle.getInstanceCount() == 0){
            m_Meshes.remove(meshDataID);
        }
    }

    MaterialID GeometryPass::createMaterial(const MaterialDesc &material) {
        auto maid = generateID();

        m_Materials.emplace(maid, m_Context, material, m_MaterialStartSlot);

        SR_POSTCONDITION(maid != 0, "MaterialID 0 is reserved for invalid material")
        return maid;
    }

    void GeometryPass::setMeshMaterial(MeshID meshID, MaterialID materialID) {
        SR_PRECONDITION(m_Materials.has(materialID), "MaterialID: %d does not exist", materialID)

        MeshDataID meshDataID = m_MeshInstances[meshID];
        m_Meshes[meshDataID].materialID = materialID;
    }

    void GeometryPass::removeMaterial(MaterialID materialID) {
        SR_PRECONDITION(m_Materials.has(materialID), "MaterialID: %d does not exist", materialID)

        m_Materials.remove(materialID);
    }

    void GeometryPass::createDefaultMaterial() {
        // 2x2 chessboard texture (black and purple)
        std::vector<uint8> albedo = {
                0, 0, 0, 255, 128, 0, 128, 255,
                128, 0, 128, 255, 0, 0, 0, 255
        };
        // 1x1 normal texture (normal pointing up)
        std::vector<uint8> normal = {
                0, 0, 255, 255
        };
        // 1x1 roughness texture (roughness 0.5)
        std::vector<uint8> roughness = {
                128, 128, 128, 255
        };
        // 1x1 metallic texture (metallic 0.5)
        std::vector<uint8> metallic = {
                128, 128, 128, 255
        };
        // 1x1 ao texture (ao 1.0)
        std::vector<uint8> ao = {
                255, 255, 255, 255
        };
        ImageUI8Desc albedoDesc;
        albedoDesc.width = 2;
        albedoDesc.height = 2;
        albedoDesc.data = albedo.data();
        albedoDesc.format = SR_TEXTURE_RGBA_UI8;

        ImageUI8Desc normalDesc;
        normalDesc.width = 1;
        normalDesc.height = 1;
        normalDesc.data = normal.data();
        normalDesc.format = SR_TEXTURE_RGBA_UI8;

        ImageUI8Desc roughnessDesc;
        roughnessDesc.width = 1;
        roughnessDesc.height = 1;
        roughnessDesc.data = roughness.data();
        roughnessDesc.format = SR_TEXTURE_RGBA_UI8;

        ImageUI8Desc metallicDesc;
        metallicDesc.width = 1;
        metallicDesc.height = 1;
        metallicDesc.data = metallic.data();
        metallicDesc.format = SR_TEXTURE_RGBA_UI8;

        ImageUI8Desc aoDesc;
        aoDesc.width = 1;
        aoDesc.height = 1;
        aoDesc.data = ao.data();
        aoDesc.format = SR_TEXTURE_RGBA_UI8;

        MaterialDesc material;
        material.albedo = createImageUI8(albedoDesc);
        material.normal = createImageUI8(normalDesc);
        material.roughness = createImageUI8(roughnessDesc);
        material.metallic = createImageUI8(metallicDesc);
        material.ao = createImageUI8(aoDesc);

        m_Materials.emplace(0, m_Context, material, m_MaterialStartSlot);

    }
}
