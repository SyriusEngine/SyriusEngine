#include <SyriusEngine/Renderer/Material.hpp>

namespace Syrius {

    Material::Material(const fs::path &albedoPath,
        const fs::path &normalPath,
        const fs::path &metallicPath,
        const fs::path &roughnessPath,
        const fs::path &aoPath) {
        m_Albedo = loadFromFile(albedoPath);
        m_Normal = loadFromFile(normalPath);
        auto metallic = loadFromFile(metallicPath);
        auto roughness = loadFromFile(roughnessPath);
        auto ao = loadFromFile(aoPath);

    }




}
