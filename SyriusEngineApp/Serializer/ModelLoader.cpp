#include <cstddef>
#include "ModelLoader.hpp"

namespace Syrius{

    void assimpLogStream(const char* message, char* user){
        SR_LOG_INFO("Assimp: %s", message);
    }

    class LogStream : public Assimp::LogStream{
    public:
        LogStream() = default;

        LogStream(void (*param)(const char *, char *), void* aNullptr){
            m_Log = param;
            m_User = aNullptr;
        }

        void write(const char* message) override{
            SR_LOG_INFO("Assimp: %s", message);
        }
    private:
        void (*m_Log)(const char*, char*);
        void* m_User;
    };

    ModelLoader::ModelLoader(const std::string& path, const ResourceView<RenderCommand>& renderCommand):
    m_RenderCommand(renderCommand),
    m_Importer(){
        Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE, aiDefaultLogStream_STDOUT);
        Assimp::DefaultLogger::get()->attachStream(new LogStream(assimpLogStream, nullptr));

        m_Scene = m_Importer.ReadFile(path, aiProcess_Triangulate |
                                          aiProcess_FlipUVs |
                                          aiProcess_CalcTangentSpace |
                                          aiProcess_GenNormals |
                                          aiProcess_JoinIdenticalVertices |
                                          aiProcess_OptimizeMeshes |
                                          aiProcess_OptimizeGraph);
        if (!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode) {
            SR_THROW("Failed to load model from file: %s", path.c_str());
        }
        auto lastSlash = path.find_last_of('/');
        auto lastBackSlash = path.find_last_of('\\');
        if (lastSlash == std::string::npos){
            m_Path = path.substr(0, lastBackSlash);
        }
        else{
            m_Path = path.substr(0, lastSlash);
        }
    }

    ModelLoader::~ModelLoader()= default;

    void ModelLoader::parse(std::vector<Resource<Mesh>>& meshes){
        processNode(m_Scene->mRootNode, meshes);
    }

    void ModelLoader::processNode(aiNode *node, std::vector<Resource<Mesh>> &meshes) {
        for (uint32 i = 0; i < node->mNumMeshes; i++){
            aiMesh* mesh = m_Scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, meshes);
        }

        for (uint32 i = 0; i < node->mNumChildren; i++){
            processNode(node->mChildren[i], meshes);
        }
    }

    void ModelLoader::processMesh(aiMesh *mesh, std::vector<Resource<Mesh>> &meshes) {
        MeshDesc desc;
        desc.vertices.reserve(mesh->mNumVertices);
        desc.indices.reserve(mesh->mNumFaces * 3);

        for (uint32 i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            if (mesh->HasNormals()){
                vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }
            if (mesh->HasTangentsAndBitangents()){
                vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            }
            if (mesh->HasTextureCoords(0)){
                vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            else{
                vertex.texCoord = glm::vec2(0.0f, 0.0f);
            }
            desc.vertices.push_back(vertex);
        }

        for (uint32 i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (uint32 j = 0; j < face.mNumIndices; j++) {
                desc.indices.push_back(face.mIndices[j]);
            }
        }

        std::string name = mesh->mName.C_Str();
        auto srMesh = createResource<Mesh>(name, desc, m_RenderCommand);
        auto mid = processMaterial(m_Scene->mMaterials[mesh->mMaterialIndex]);
        srMesh->setMaterial(mid);

        meshes.push_back(std::move(srMesh));

    }

    MaterialID ModelLoader::processMaterial(aiMaterial *material) {
        if (m_Materials.find(material) != m_Materials.end()){
            return m_Materials[material];
        }
        else{
            printf("Fix Assimp material loading\n");
        }
        return 0;
    }

}
