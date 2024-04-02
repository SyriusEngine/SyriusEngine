#include "MaterialHandle.hpp"

namespace Syrius{

    MaterialHandle::MaterialHandle(ResourceView<Context> &context, const MaterialDesc &desc, uint32 slot):
    m_Context(context),
    m_Slot(slot){
        validateImages(desc);
        auto width = desc.albedo->getWidth();
        if (width * 5 > context->getDeviceLimits()->getMaxTextureSize()){
            SR_LOG_ERROR("Material textures are too large, maximum texture size is: %d", context->getDeviceLimits()->getMaxTextureSize());
        }
        // create texture that contains all material textures
        Texture2DDesc texDesc;
        texDesc.width = width * 5;
        texDesc.height = desc.albedo->getHeight();
        texDesc.format = desc.albedo->getFormat();
        texDesc.data = desc.albedo->getData(); // immediately copy albedo data
        m_Material = context->createTexture2D(texDesc);

        // copy other textures to the material texture
        m_Material->setData(desc.normal->getData(), width, 0, desc.normal->getWidth(), desc.normal->getHeight());
        m_Material->setData(desc.metallic->getData(), width * 2, 0, desc.metallic->getWidth(), desc.metallic->getHeight());
        m_Material->setData(desc.roughness->getData(), width * 3, 0, desc.roughness->getWidth(), desc.roughness->getHeight());
        m_Material->setData(desc.ao->getData(), width * 4, 0, desc.ao->getWidth(), desc.ao->getHeight());
    }

    MaterialHandle::MaterialHandle(MaterialHandle &&other) noexcept:
    m_Context(other.m_Context),
    m_Slot(other.m_Slot),
    m_Material(other.m_Material){

    }

    MaterialHandle &MaterialHandle::operator=(MaterialHandle &&other) noexcept {
        if (this == &other){
            return *this;
        }
        m_Context = other.m_Context;
        m_Slot = other.m_Slot;
        m_Material = other.m_Material;
        return *this;
    }

    MaterialHandle::~MaterialHandle() {

    }

    void MaterialHandle::bind() {
        m_Material->bindShaderResource(m_Slot);
    }

    void MaterialHandle::validateImages(const MaterialDesc &desc) {
        auto format = desc.albedo->getFormat();
        if (format != desc.normal->getFormat() || format != desc.metallic->getFormat() || format != desc.roughness->getFormat() || format != desc.ao->getFormat()){
            SR_LOG_ERROR("Material textures have different formats, selected format is: %d", format);
        }
        auto width = desc.albedo->getWidth();
        if (width != desc.normal->getWidth() || width != desc.metallic->getWidth() || width != desc.roughness->getWidth() || width != desc.ao->getWidth()){
            SR_LOG_ERROR("Material textures have different widths, selected width is: %d", width);
        }
        auto height = desc.albedo->getHeight();
        if (height != desc.normal->getHeight() || height != desc.metallic->getHeight() || height != desc.roughness->getHeight() || height != desc.ao->getHeight()){
            SR_LOG_ERROR("Material textures have different heights, selected height is: %d", height);
        }
    }

}
