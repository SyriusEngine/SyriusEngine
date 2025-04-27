#include "MaterialHandle.hpp"

namespace Syrius::Renderer {

    MaterialHandle::MaterialHandle(const MaterialID materialID, const ResourceView<Context> &ctx, const Material &material):
    m_MaterialID(materialID){
        loadTexture(ctx, material.getAlbedo(), m_Albedo);
        loadTexture(ctx, material.getNormal(), m_Normal);
        loadTexture(ctx, material.getMRAO(), m_MRAO);

        SR_LOG_INFO("MaterialHandle", "Material with ID {} created", materialID);
    }

    MaterialHandle::MaterialHandle(MaterialHandle &&other) noexcept :
        m_Albedo(std::move(other.m_Albedo)),
        m_Normal(std::move(other.m_Normal)),
        m_MRAO(std::move(other.m_MRAO)) {
    }

    MaterialHandle& MaterialHandle::operator=(MaterialHandle &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        m_Albedo = std::move(other.m_Albedo);
        m_Normal = std::move(other.m_Normal);
        m_MRAO = std::move(other.m_MRAO);
        return *this;
    }

    void MaterialHandle::bind(const u32 startSlot) const {
        m_Albedo->bindShaderResource(startSlot);
        m_Normal->bindShaderResource(startSlot + 1);
        m_MRAO->bindShaderResource(startSlot + 2);
    }

    void MaterialHandle::loadTexture(const ResourceView<Context> &ctx, const UP<Image> &image,
                                     ResourceView<Texture2D> &textureView) {
        Texture2DImageDesc desc;
        desc.image = createResourceView(image);
        textureView = ctx->createTexture2D(desc);
    }

}