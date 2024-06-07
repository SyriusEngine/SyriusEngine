#include "../../../include/SyriusEngine/Renderer/MaterialDesc.hpp"

namespace Syrius{

    MaterialDesc::MaterialDesc():
    m_Albedo(nullptr), m_Normal(nullptr), m_MRAO(nullptr),
    m_Width(0),
    m_Height(0),
    m_Format(SR_TEXTURE_NONE){

    }

    MaterialDesc::~MaterialDesc() = default;

    void MaterialDesc::loadFromFile(const std::string &albedoPath, const std::string &normalPath,
                                    const std::string &metallicPath, const std::string &roughnessPath,
                                    const std::string &aoPath) {
        SR_LOG_INFO("Loading material from files: %s, %s, %s, %s, %s\n"
                    "To increase performance, consider stitching metallic roughness and AO data into a single texture, "
                    "this process will happen automatically, but comes with a performance cost", albedoPath.c_str(), normalPath.c_str(), metallicPath.c_str(), roughnessPath.c_str(), aoPath.c_str());
        m_Albedo = loadImageFromFile(albedoPath);
        m_Normal = loadImageFromFile(normalPath);
        auto metallic = loadImageFromFile(metallicPath);
        auto roughness = loadImageFromFile(roughnessPath);
        auto ao = loadImageFromFile(aoPath);
        m_MRAO = createMRAO(metallic, roughness, ao);
    }

    void MaterialDesc::setAlbedo(Resource<Image> albedo) {
        if (m_Width == 0 or m_Height == 0 or m_Format == SR_TEXTURE_NONE){
            m_Width = albedo->getWidth();
            m_Height = albedo->getHeight();
            m_Format = albedo->getFormat();
        }
        else if (m_Width != albedo->getWidth() or m_Height != albedo->getHeight() or m_Format != albedo->getFormat()){
            SR_LOG_WARNING("Image %p has different dimensions or format than the previous images, resizing to match", albedo.get());
            albedo->resize(m_Width, m_Height);
        }
        m_Albedo = std::move(albedo);
    }

    void MaterialDesc::setNormal(Resource<Image> normal) {
        if (m_Width == 0 or m_Height == 0 or m_Format == SR_TEXTURE_NONE){
            m_Width = normal->getWidth();
            m_Height = normal->getHeight();
            m_Format = normal->getFormat();
        }
        else if (m_Width != normal->getWidth() or m_Height != normal->getHeight() or m_Format != normal->getFormat()){
            SR_LOG_WARNING("Image %p has different dimensions or format than the previous images, resizing to match", normal.get());
            normal->resize(m_Width, m_Height);
        }
        m_Normal = std::move(normal);
    }

    void MaterialDesc::setMRAO(Resource<Image> mrao) {
        if (m_Width == 0 or m_Height == 0 or m_Format == SR_TEXTURE_NONE){
            m_Width = mrao->getWidth();
            m_Height = mrao->getHeight();
            m_Format = mrao->getFormat();
        }
        else if (m_Width != mrao->getWidth() or m_Height != mrao->getHeight() or m_Format != mrao->getFormat()){
            SR_LOG_WARNING("Image %p has different dimensions or format than the previous images, resizing to match", mrao.get());
            mrao->resize(m_Width, m_Height);
        }
        m_MRAO = std::move(mrao);
    }

    const Resource<Image> &MaterialDesc::getAlbedo() const {
        return m_Albedo;
    }

    const Resource<Image> &MaterialDesc::getNormal() const {
        return m_Normal;
    }

    const Resource<Image> &MaterialDesc::getMRAO() const {
        return m_MRAO;
    }

    uint32 MaterialDesc::getWidth() const {
        return m_Width;
    }

    uint32 MaterialDesc::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT MaterialDesc::getFormat() const {
        return m_Format;
    }

    Resource<Image> MaterialDesc::loadImageFromFile(const std::string &path) {
        if (path.empty()){
            SR_LOG_WARNING("Failed to load texture from path %s", path.c_str());
            std::vector<uint8> data(4, 255);
            ImageUI8Desc desc;
            desc.width = 1;
            desc.height = 1;
            desc.format = SR_TEXTURE_RGBA_UI8;
            desc.data = data.data();
            auto image = createImage(desc);
            return std::move(image);
        }
        else{
            ImageFileDesc imageDesc;
            imageDesc.fileName = path;
            imageDesc.flipOnAccess = true;
            imageDesc.requestedChannelCount = 4;
            auto image = createImage(imageDesc);
            if (m_Width == 0 or m_Height == 0 or m_Format == SR_TEXTURE_NONE){
                m_Width = image->getWidth();
                m_Height = image->getHeight();
                m_Format = image->getFormat();
            }
            else{
                if (m_Width != image->getWidth() or m_Height != image->getHeight() or m_Format != image->getFormat()){
                    SR_LOG_WARNING("Image %s has different dimensions or format than the previous images, resizing to match", path.c_str());
                    image->resize(m_Width, m_Height);
                }
            }
            return std::move(image);
        }
    }

    Resource<Image> MaterialDesc::createMRAO(const Resource<Image> &metallic, const Resource<Image> &roughness, const Resource<Image> &ao) {
        SR_PRECONDITION(m_Width != 0, "Width must be greater than 0");
        SR_PRECONDITION(m_Height != 0, "Height must be greater than 0");
        SR_PRECONDITION(m_Format != SR_TEXTURE_NONE, "Format must be valid");

        ImageUI8Desc desc;
        desc.width = m_Width;
        desc.height = m_Height;
        desc.format = m_Format;
        auto mrao = createImage(desc);
        auto mraoPtr = reinterpret_cast<uint8*>(mrao->getData());
        auto metallicPtr = reinterpret_cast<uint8*>(metallic->getData());
        auto roughnessPtr = reinterpret_cast<uint8*>(roughness->getData());
        auto aoPtr = reinterpret_cast<uint8*>(ao->getData());
        auto size = m_Width * m_Height * 4;
        for (Size i = 0; i < size; i += 4){
            mraoPtr[i] = metallicPtr[i];
            mraoPtr[i + 1] = roughnessPtr[i];
            mraoPtr[i + 2] = aoPtr[i];
            mraoPtr[i + 3] = 255;
        }

        return std::move(mrao);
    }


}
