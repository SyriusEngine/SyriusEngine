#pragma once

#include "../Include.hpp"
#include "../SceneObjects/Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>

namespace Syrius {

    class ModelLoader {
    public:
        ModelLoader(const std::string &path, const ResourceView<RenderCommand>& rd);

        ~ModelLoader();

        void parse(std::vector<Resource<Mesh>> &meshes);

    private:
        void processNode(aiNode *node, std::vector<Resource<Mesh>> &meshes);

        void processMesh(aiMesh *mesh, std::vector<Resource<Mesh>> &meshes);

        MaterialID processMaterial(aiMaterial* material);

    private:
        const ResourceView<RenderCommand>& m_RenderCommand;
        const aiScene* m_Scene;
        Assimp::Importer m_Importer;
        std::string m_Path;

        std::unordered_map<aiMaterial*, MaterialID> m_Materials;


    };
}
