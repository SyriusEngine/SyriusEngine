#pragma once

#include "../Include.hpp"
#include "../SceneObjects/Mesh.hpp"
#include "../SceneObjects/LightObject.hpp"

namespace Syrius{

    class Serializer{
    public:
        explicit Serializer(const ResourceView<RenderCommand>& renderCommand, std::string materialDir = "./Resources/Materials/");

        ~Serializer();

        void loadScene(const std::string& iniPath);

        void clear();

        MaterialID getMaterial(const std::string& materialName);

        [[nodiscard]] inline std::vector<Resource<Mesh>>& getMeshes() { return m_Meshes; }

        [[nodiscard]] inline std::vector<Resource<LightObject>>& getLights() { return m_Lights; }

    private:

        void privateLoadScene(const std::string& iniPath);

        void loadLight(EasyIni::Section& section);

        void loadMesh(EasyIni::Section& section);

        MaterialID privateGetMaterial(const std::string& materialName);

    private:
        Worker m_Dispatcher;
        const ResourceView<RenderCommand>& m_RenderCommand;
        std::string m_MaterialDir;

        std::unordered_map<std::string, MaterialID> m_Materials;

        std::vector<Resource<Mesh>> m_Meshes;
        std::vector<Resource<LightObject>> m_Lights;


    };

}