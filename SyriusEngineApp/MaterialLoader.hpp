#pragma once

#include "Include.hpp"

namespace Syrius{

    class MaterialLoader{
    public:
        explicit MaterialLoader(const ResourceView<RenderCommand>& renderCommand, std::string materialDir = "./Resources/Materials/");

        MaterialID getMaterial(const std::string& materialName);

    private:
        const ResourceView<RenderCommand>& m_RenderCommand;
        std::unordered_map<std::string, MaterialID> m_Materials;

        std::string m_MaterialDir;
    };
}