#include "MaterialHandle.hpp"

namespace Syrius{

    MaterialHandle::MaterialHandle(ResourceView<Context> &context, const MaterialDesc &desc, uint32 slot):
    m_Context(context),
    m_Slot(slot){
        auto width = desc.getWidth();
        if (width * 3 > context->getDeviceLimits()->getMaxTextureSize()){
            SR_LOG_ERROR("Material textures are too large, maximum texture size is: %d", context->getDeviceLimits()->getMaxTextureSize());
        }

        Texture2DImageDesc albedoDesc;
        albedoDesc.image = createResourceView(desc.getAlbedo());
        m_Albedo = context->createTexture2D(albedoDesc);

        Texture2DImageDesc normalDesc;
        normalDesc.image = createResourceView(desc.getNormal());
        m_Normal = context->createTexture2D(normalDesc);

        Texture2DImageDesc mraoDesc;
        mraoDesc.image = createResourceView(desc.getMRAO());
        m_MRAO = context->createTexture2D(mraoDesc);
    }

    MaterialHandle::MaterialHandle(MaterialHandle &&other) noexcept:
    m_Context(other.m_Context),
    m_Slot(other.m_Slot),
    m_Albedo(std::move(other.m_Albedo)),
    m_Normal(std::move(other.m_Normal)),
    m_MRAO(std::move(other.m_MRAO)){

    }

    MaterialHandle &MaterialHandle::operator=(MaterialHandle &&other) noexcept {
        if (this == &other){
            return *this;
        }
        m_Context = other.m_Context;
        m_Slot = other.m_Slot;
        m_Albedo = std::move(other.m_Albedo);
        m_Normal = std::move(other.m_Normal);
        m_MRAO = std::move(other.m_MRAO);
        return *this;
    }

    MaterialHandle::~MaterialHandle() = default;

    void MaterialHandle::bind() {
        m_Albedo->bindShaderResource(m_Slot);
        m_Normal->bindShaderResource(m_Slot + 1);
        m_MRAO->bindShaderResource(m_Slot + 2);
    }
}
