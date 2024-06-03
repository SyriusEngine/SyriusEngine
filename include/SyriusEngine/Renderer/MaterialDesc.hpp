#pragma once

#include "Primitives.hpp"

namespace Syrius{

    class SR_API MaterialDesc{
    public:
        MaterialDesc();

        ~MaterialDesc();

        void loadFromFile(const std::string& albedoPath,
                          const std::string& normalPath,
                          const std::string& metallicPath,
                          const std::string& roughnessPath,
                          const std::string& aoPath);

        void setAlbedo(Resource<Image> albedo);

        void setNormal(Resource<Image> normal);

        void setMRAO(Resource<Image> mrao);

        [[nodiscard]] const Resource<Image>& getAlbedo() const;

        [[nodiscard]] const Resource<Image>& getNormal() const;

        [[nodiscard]] const Resource<Image>& getMRAO() const;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

    private:

        Resource<Image> loadImageFromFile(const std::string& path);

        Resource<Image> createMRAO(const Resource<Image>& metallic, const Resource<Image>& roughness, const Resource<Image>& ao);


    private:
        Resource<Image> m_Albedo;
        Resource<Image> m_Normal;
        Resource<Image> m_MRAO; // combines metallic, roughness and ambient occlusion in a single texture, limiting texture accesses in the shader since they are stored interleaved

        // since we will combine all images into a single texture, each part needs to have the same dimensions and format
        uint32 m_Width;
        uint32 m_Height;
        SR_TEXTURE_FORMAT m_Format;
    };

}