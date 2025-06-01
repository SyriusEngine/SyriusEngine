#include "MaterialStore.hpp"
#include "../RenderGraphContainer.hpp"

namespace Syrius::Renderer {

    MaterialStore::MaterialStore(const ResourceView<Context>& ctx, RenderGraphContainer* container, const SP<DispatcherManager>& dispatcherManager):
    IRenderGraphData(ctx, container){
        createDefaultMaterial();

        // When a new mesh is created, let the mesh use the default Material until a new materialID is dispatched
        const auto meshDispatcher = dispatcherManager->getDispatcher<MeshID, Mesh>();
        meshDispatcher->registerCreate([this](const MeshID meshID, const SP<Mesh> &mesh) {
            m_MeshToMaterial[meshID] = SR_DEFAULT_MATERIAL;
        }, SR_WORKER_RENDERER);
        meshDispatcher->registerDelete([this](const MeshID meshID) {
            m_MeshToMaterial.erase(meshID);
        }, SR_WORKER_RENDERER);

        const auto materialDispatcher = dispatcherManager->getDispatcher<MaterialID, Material>();
        materialDispatcher->registerCreate([this](const MaterialID materialID, const SP<Material>& material) {
            createMaterial(materialID, material);
        }, SR_WORKER_RENDERER);
        materialDispatcher->registerDelete([this](const MaterialID materialID) {
            destroyMaterial(materialID);
        }, SR_WORKER_RENDERER);

        const auto meshMaterialDispatcher = dispatcherManager->getDispatcher<MeshID, MaterialID>();
        meshMaterialDispatcher->registerUpdate([this](const MeshID meshID, const SP<MaterialID>& materialID) {
            m_MeshToMaterial[meshID] = *materialID;
        }, SR_WORKER_RENDERER);

        SR_LOG_INFO("MaterialStore", "MaterialStore Created!");
    }

    void MaterialStore::createMaterial(MaterialID materialID, const SP<Material> &material) {
        if (m_Materials.has(materialID)) {
            SR_LOG_WARNING("MaterialStore", "Material {} already created", materialID);
            return;
        }
        m_Materials.emplace(materialID, materialID, m_Ctx, *material);
        SR_LOG_INFO("MaterialStore", "Material {} created", materialID);

        SR_POSTCONDITION(m_Materials.has(materialID), "Material {} not created", materialID);
    }

    void MaterialStore::destroyMaterial(MaterialID materialID) {
        if (!m_Materials.has(materialID)) {
            SR_LOG_WARNING("MaterialStore", "Material {} does not exist!", materialID);
            return;
        }
        m_Materials.remove(materialID);

        SR_POSTCONDITION(!m_Materials.has(materialID), "Material {} not destroyed", materialID);
    }

    void MaterialStore::bindMeshMaterial(const MeshID meshID, const u32 startSlot) {
        SR_PRECONDITION(m_MeshToMaterial.find(meshID) != m_MeshToMaterial.end(), "Mesh {} does not exist in the materialStore!", meshID);

        const MaterialID materialID = m_MeshToMaterial.at(meshID);
        // TODO: What happens if a material does not exist anymore?
        m_Materials[materialID].bind(startSlot);
    }

    void MaterialStore::createDefaultMaterial() {
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

        Material defaultMaterial(std::move(albedoImage), std::move(normalImage), std::move(mraoImage));
        m_Materials.emplace(SR_DEFAULT_MATERIAL, SR_DEFAULT_MATERIAL, m_Ctx, defaultMaterial);
    }


}
