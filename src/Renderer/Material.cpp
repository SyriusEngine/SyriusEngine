#include <SyriusEngine/Renderer/Material.hpp>

namespace Syrius {

    Material::Material(const fs::path &albedoPath,
        const fs::path &normalPath,
        const fs::path &metallicPath,
        const fs::path &roughnessPath,
        const fs::path &aoPath) {
        if (!fs::exists(albedoPath)) {
            m_Width = 1;
            m_Height = 1;
            m_Format = SR_TEXTURE_RGBA_UI8;
        }
        m_Albedo = loadFromFile(albedoPath);
        m_Normal = loadFromFile(normalPath);
        auto metallic = loadFromFile(metallicPath);
        auto roughness = loadFromFile(roughnessPath);
        auto ao = loadFromFile(aoPath);
        SR_LOG_PERFORMANCE("Metallic, roughness and ambient occlusion textures are provided as individual textures."
                       "To improve performance, these will be combined into a single texture, but consider"
                       "doing this beforehand to reduce material load times.", "TODO: Should not be printed");

    }

    Material::Material(const fs::path &albedoPath, const fs::path &normalPath, const fs::path &mraoPath) {
        if (!fs::exists(albedoPath)) {
            m_Width = 1;
            m_Height = 1;
            m_Format = SR_TEXTURE_RGBA_UI8;
        }
        m_Albedo = loadFromFile(albedoPath);
        m_Normal = loadFromFile(normalPath);
        m_MRAO = loadFromFile(mraoPath);
    }


    UP<Image> Material::loadFromFile(const fs::path &path) {
        if (!fs::exists(path)) {
            SR_LOG_WARNING("Material", "Unable to load material from file: {}", path.string());
            return createDefaultImage();
        }
        ImageFileDesc fileDesc;
        fileDesc.requestedChannelCount = 4; // GPUs like RGBA
        fileDesc.fileName = path;
        fileDesc.flipOnAccess = true; // Flip all images by default
        auto image = createImage(fileDesc);
        // We loaded the first image
        if (m_Width == 0 || m_Height == 0 || m_Format == SR_TEXTURE_NONE) {
            m_Width = image->getWidth();
            m_Height = image->getHeight();
            m_Format = image->getFormat();
            return std::move(image);
        }

        // The dimensions of the image must match the first image!
        if (image->getWidth() != m_Width || image->getHeight() != m_Height) {
            SR_LOG_PERFORMANCE("Material", "Image {} has dimensions {}x{}, but the first image has dimensions {}x{}"
                                       "The image will be resized, but to improve performance, resize the image"
                                       " beforehand", path.string(), image->getWidth(), image->getHeight(),
                                       m_Width, m_Height);
            image->resize(m_Width, m_Height);
        }
        // Format must also match!
        if (image->getFormat() != m_Format) {
            SR_LOG_WARNING("Material", "Image {} has format {}, but the first image has format {}"
                                       "The default image will be used!", path.string(), image->getFormat());
            return createDefaultImage();
        }
        return std::move(image);
    }

    UP<Image> Material::createDefaultImage() const {
        std::vector<u8> defaultColor(m_Width * m_Height * getTextureChannelCount(m_Format), 255);
        ImageUI8Desc desc;
        desc.data = defaultColor.data();
        desc.width = m_Width;
        desc.height = m_Height;
        desc.format = m_Format;
        return createImage(desc);
    }

    bool checkDimensions(const UP<Image> &metallic, const UP<Image> &roughness, const UP<Image> &ao) {
        if (metallic->getWidth() != roughness->getWidth() || metallic->getHeight() != roughness->getHeight()) {
            return false;
        }
        if (metallic->getWidth() != ao->getWidth() || metallic->getHeight() != ao->getHeight()) {
            return false;
        }
        return true;
    }

    UP<Image> Material::createMRAO(const UP<Image> &metallic, const UP<Image> &roughness, const UP<Image> &ao) const {
        if (!checkDimensions(metallic, roughness, ao)) {
            SR_LOG_WARNING("Material", "Metallic, roughness and ambient occlusion textures have different dimensions. "
                                       "The first image will be used as the base.");
            return createDefaultImage();
        }
        ImageUI8Desc desc;
        desc.width = m_Width;
        desc.height = m_Height;
        desc.format = SR_TEXTURE_RGBA_UI8;
        auto mrao = createImage(desc);
        auto mraoPtr = reinterpret_cast<u8*>(mrao->getData());
        auto metallicPtr = reinterpret_cast<u8*>(metallic->getData());
        auto roughnessPtr = reinterpret_cast<u8*>(roughness->getData());
        auto aoPtr = reinterpret_cast<u8*>(ao->getData());
        Size imageSize = m_Width * m_Height * getTextureChannelCount(desc.format);
        for (Size i = 0; i < imageSize; i += 4) {
            mraoPtr[i] = metallicPtr[i];
            mraoPtr[i + 1] = roughnessPtr[i];
            mraoPtr[i + 2] = aoPtr[i];
            mraoPtr[i + 3] = 255;
        }
        return std::move(mrao);
    }


}
