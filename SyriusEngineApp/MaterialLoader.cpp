#include "MaterialLoader.hpp"

#include <utility>

namespace Syrius{

    MaterialLoader::MaterialLoader(const ResourceView<RenderCommand>& renderCommand, std::string materialDir):
    m_RenderCommand(renderCommand),
    m_MaterialDir(std::move(materialDir)){
    }

    MaterialID MaterialLoader::getMaterial(const std::string& materialName){
        auto it = m_Materials.find(materialName);
        if(it != m_Materials.end()){
            return it->second;
        }

        ImageFileDesc albedoDesc;
        albedoDesc.fileName = m_MaterialDir + materialName + "/" + materialName + "_basecolor.png";
        ImageFileDesc normalDesc;
        normalDesc.fileName = m_MaterialDir + materialName + "/" + materialName + "_normal.png";
        ImageFileDesc metallicDesc;
        metallicDesc.fileName = m_MaterialDir + materialName + "/" + materialName + "_metallic.png";
        ImageFileDesc roughnessDesc;
        roughnessDesc.fileName = m_MaterialDir + materialName + "/" + materialName + "_roughness.png";
        ImageFileDesc aoDesc;
        aoDesc.fileName = m_MaterialDir + materialName + "/" + materialName + "_ao.png";

        MaterialDesc material;
        material.albedo = createImage(albedoDesc);
        material.albedo->extendAlpha();
        material.normal = createImage(normalDesc);
        material.normal->extendAlpha();
        material.metallic = createImage(metallicDesc);
        material.metallic->extendAlpha();
        material.roughness = createImage(roughnessDesc);
        material.roughness->extendAlpha();
        material.ao = createImage(aoDesc);
        material.ao->extendAlpha();

        MaterialID mid = m_RenderCommand->createMaterial(material);
        m_Materials[materialName] = mid;
        return mid;
    }
}
