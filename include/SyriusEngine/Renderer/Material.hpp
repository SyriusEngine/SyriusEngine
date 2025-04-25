#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

namespace Syrius {

    class SR_ENGINE_API Material {
    public:
        Material(const fs::path& albedoPath,
                 const fs::path& normalPath,
                 const fs::path& metallicPath,
                 const fs::path& roughnessPath,
                 const fs::path& aoPath);

        Material(const fs::path& albedoPath,
                 const fs::path& normalPath,
                 const fs::path& mraoPath);

        ~Material() = default;

        const UP<Image>& getAlbedo() const { return m_Albedo; }

        const UP<Image>& getNormal() const { return m_Normal; }

        const UP<Image>& getMRAO() const { return m_MRAO;}

    private:

        UP<Image> loadFromFile(const fs::path& path);

        UP<Image> createDefaultImage() const;

        UP<Image> createMRAO(const UP<Image>& metallic, const UP<Image>& roughness, const UP<Image>& ao) const;

    private:
        // For easy use, enforce that every texture has the same dimensions and format
        u32 m_Width = 0;
        u32 m_Height = 0;
        SR_TEXTURE_FORMAT m_Format = SR_TEXTURE_NONE;

        UP<Image> m_Albedo;
        UP<Image> m_Normal;
        UP<Image> m_MRAO; // combines metallic, roughness and ambient occlusion in a single texture, limiting texture accesses in the shader since they are stored interleaved
    };

}