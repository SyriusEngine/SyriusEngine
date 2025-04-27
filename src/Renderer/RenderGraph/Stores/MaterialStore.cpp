#include "MaterialStore.hpp"

namespace Syrius::Renderer {

    MaterialStore::MaterialStore(const ResourceView<Context>& ctx) {
        // 2x2 chessboard texture (black and purple)
        std::vector<u8> albedo = {
            0, 0, 0, 255, 128, 0, 128, 255,
            128, 0, 128, 255, 0, 0, 0, 255
        };
        // 1x1 normal texture (normal pointing up)
        std::vector<u8> normal = {
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
        m_Materials.emplace(SR_DEFAULT_MATERIAL, SR_DEFAULT_MATERIAL, ctx, defaultMaterial);

        SR_LOG_INFO("MaterialStore", "MaterialStore Created!");
    }

    void MaterialStore::createMaterial(MaterialID materialID, const Material &material,
                                       const ResourceView<Context> &ctx) {
        if (m_Materials.has(materialID)) {
            SR_LOG_WARNING("MaterialStore", "Material {} already created", materialID);
            return;
        }
        m_Materials.emplace(materialID, materialID, ctx, material);

        SR_POSTCONDITION(m_Materials.has(materialID), "Material {} not created", materialID);
    }

    void MaterialStore::destroyMaterial(MaterialID materialID, const ResourceView<Context> &ctx) {
        if (!m_Materials.has(materialID)) {
            SR_LOG_WARNING("MaterialStore", "Material {} does not exist!", materialID);
            return;
        }
        m_Materials.remove(materialID);

        SR_POSTCONDITION(!m_Materials.has(materialID), "Material {} not destroyed", materialID);
    }

    void MaterialStore::bindMaterial(MaterialID materialID, const u32 startSlot) {
        SR_PRECONDITION(m_Materials.has(materialID), "Material {} does not exist!", materialID);

        m_Materials[materialID].bind(startSlot);
    }

}
