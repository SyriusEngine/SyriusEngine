#include "Serializer.hpp"
#include <EasyIni/EasyIni.hpp>

#include "ModelLoader.hpp"

namespace Syrius{

    Serializer::Serializer(const ResourceView<RenderCommand> &renderCommand, std::string materialDir):
    m_RenderCommand(renderCommand),
    m_MaterialDir(std::move(materialDir)),
    m_Dispatcher("AppLayer::Serializer"){

    }

    Serializer::~Serializer() {
        m_Lights.clear();
        m_Meshes.clear();
        m_Dispatcher.stop();
    }

    void Serializer::loadScene(const std::string &iniPath) {
        std::cout << "Loading scene " << iniPath << std::endl;
        m_Dispatcher.addTask([this, iniPath](){
            privateLoadScene(iniPath);
        });
    }

    void Serializer::clear() {
        m_Dispatcher.addTaskSync([this](){
            m_Lights.clear();
            m_Meshes.clear();
        });
    }

    MaterialID Serializer::getMaterial(const std::string &materialName) {
        MaterialID mid = 0;
        m_Dispatcher.addTaskSync([this, &mid, &materialName](){
            mid = privateGetMaterial(materialName);
        });
        return mid;
    }

    void Serializer::privateLoadScene(const std::string &iniPath) {
        EasyIni::Configuration sceneConfig(iniPath);

        auto objectCount = sceneConfig["General"]["ObjectCount"].get<uint32>();
        auto name = sceneConfig["General"]["Name"].get<std::string>();
        for (uint32 i = 0; i < objectCount; i++){
            std::string sectionName = "Object" + std::to_string(i);
            auto& section = sceneConfig[sectionName];
            auto type = section["Type"].get<std::string>();
            if (type == "Light"){
                loadLight(section);
            } else if (type == "Mesh"){
                loadMesh(section);
            }
        }
        std::cout << "[Serializer]: Successfully loaded scene " << name << std::endl;
    }

    void Serializer::loadLight(EasyIni::Section &section) {
        auto position = section["Position"].getVector<float>();
        SR_ASSERT(position.size() == 3, "Light position must have 3 components, has %i", position.size());
        auto color = section["Color"].getVector<float>();
        SR_ASSERT(color.size() == 3, "Light color must have 3 components, has %i", color.size());
        auto lightObject = createResource<LightObject>(m_RenderCommand, glm::vec3(position[0], position[1], position[2]), glm::vec3(color[0], color[1], color[2]));
        m_Lights.push_back(std::move(lightObject));
    }

    void Serializer::loadMesh(EasyIni::Section &section) {
        auto name = section["Name"].get<std::string>();
        auto meshType = section["MeshType"].get<std::string>();
        if (meshType == "Prefab"){
            loadPrefab(section, name);
        }
        else if (meshType == "Instance"){
            loadInstance(section, name);
        }
        else if (meshType == "3DModel"){
            load3DModel(section, name);
        }
        else{
            SR_THROW("Unknown mesh type %s", meshType.c_str());
        }
        auto position = section["Position"].getVector<float>();
        SR_ASSERT(position.size() == 3, "Mesh position must have 3 components, has %i", position.size());
        auto rotation = section["Rotation"].getVector<float>();
        SR_ASSERT(rotation.size() == 3, "Mesh rotation must have 3 components, has %i", rotation.size());
        auto scale = section["Scale"].getVector<float>();
        SR_ASSERT(scale.size() == 3, "Mesh scale must have 3 components, has %i", scale.size());

        auto& mesh = m_Meshes.back();
        mesh->setTranslation(glm::vec3(position[0], position[1], position[2]));
        mesh->setRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));
        mesh->setScale(glm::vec3(scale[0], scale[1], scale[2]));

    }

    MaterialID Serializer::privateGetMaterial(const std::string &materialName) {
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
        //material.albedo->resize(512, 512);
        material.normal = createImage(normalDesc);
        material.normal->extendAlpha();
        //material.normal->resize(512, 512);
        material.metallic = createImage(metallicDesc);
        material.metallic->extendAlpha();
        //material.metallic->resize(512, 512);
        material.roughness = createImage(roughnessDesc);
        material.roughness->extendAlpha();
        //material.roughness->resize(512, 512);
        material.ao = createImage(aoDesc);
        material.ao->extendAlpha();
        //material.ao->resize(512, 512);

        MaterialID mid = m_RenderCommand->createMaterial(material);
        m_Materials[materialName] = mid;
        return mid;
    }

    void Serializer::loadPrefab(EasyIni::Section &section, const std::string &name) {
        auto prefab = section["PrefabType"].get<std::string>();
        auto material = section["Material"].get<std::string>();
        MaterialID mid = privateGetMaterial(material);

        MeshDesc meshDesc;
        if (prefab == "Triangle"){
            createTriangle(meshDesc);
        }
        else if (prefab == "Rectangle"){
            createRectangle(meshDesc);
        }
        else if (prefab == "Pyramid"){
            createPyramid(meshDesc);
        }
        else if (prefab == "Cube"){
            createCube(meshDesc);
        }
        else if (prefab == "Sphere"){
            createSphere(meshDesc);
        }
        else if (prefab == "Cone"){
            createCone(meshDesc);
        }
        else if (prefab == "Torus"){
            createTorus(meshDesc);
        }
        else if (prefab == "Cylinder"){
            createCylinder(meshDesc);
        }
        else{
            SR_THROW("Unknown prefab %s", prefab.c_str());
        }
        auto mesh = createResource<Mesh>(name, meshDesc, m_RenderCommand);
        mesh->setMaterial(mid);
        m_Meshes.push_back(std::move(mesh));
    }

    void Serializer::loadInstance(EasyIni::Section &section, const std::string &name) {
        auto instancedFrom = section["InstancedFrom"].get<std::string>();

        for (auto& mesh : m_Meshes) {
            if (mesh == nullptr) continue; // not sure why this is necessary, but engine crashes without it
            if (mesh->getName() == instancedFrom) {
                auto id = mesh->getMeshID();
                auto newMesh = createResource<Mesh>(name, id, m_RenderCommand);
                m_Meshes.push_back(std::move(newMesh));
            }
        }
    }

    void Serializer::load3DModel(EasyIni::Section &section, const std::string &name) {
        auto file = section["FilePath"].get<std::string>();
        ModelLoader loader(file, m_RenderCommand);
        loader.parse(m_Meshes);
    }
}
