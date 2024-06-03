#include "MaterialHandle.hpp"

namespace Syrius{

    MaterialHandle::MaterialHandle(ResourceView<Context> &context, const MaterialDesc &desc, uint32 slot):
    m_Context(context),
    m_Slot(slot){
        auto width = desc.getWidth();
        if (width * 3 > context->getDeviceLimits()->getMaxTextureSize()){
            SR_LOG_ERROR("Material textures are too large, maximum texture size is: %d", context->getDeviceLimits()->getMaxTextureSize());
        }
        // create texture that contains all material textures
        Texture2DDesc texDesc;
        texDesc.width = width * 3;
        texDesc.height = desc.getHeight();
        texDesc.format = desc.getFormat();
        texDesc.data = nullptr;
        m_Material = context->createTexture2D(texDesc);

        // copy other textures to the material texture
        m_Material->setData(desc.getAlbedo()->getData(), 0, 0, width, desc.getHeight());
        m_Material->setData(desc.getNormal()->getData(), width, 0, width, desc.getHeight());
        m_Material->setData(desc.getMRAO()->getData(), width * 2, 0, width, desc.getHeight());
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
}
