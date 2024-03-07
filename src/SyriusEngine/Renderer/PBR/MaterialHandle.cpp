#include "MaterialHandle.hpp"

namespace Syrius{

    MaterialHandle::MaterialHandle(ResourceView<Context> &context, const MaterialDesc &desc, uint32 startingSlot):
    m_Context(context),
    m_StartingSlot(startingSlot){
        createTexture2D(desc.albedo, m_Albedo);
        createTexture2D(desc.normal, m_Normal);
        createTexture2D(desc.metallic, m_Metallic);
        createTexture2D(desc.roughness, m_Roughness);
        createTexture2D(desc.ao, m_Ao);
    }

    MaterialHandle::MaterialHandle(MaterialHandle &&other) noexcept:
    m_Context(other.m_Context),
    m_Albedo(other.m_Albedo),
    m_Normal(other.m_Normal),
    m_Metallic(other.m_Metallic),
    m_Roughness(other.m_Roughness),
    m_Ao(other.m_Ao),
    m_StartingSlot(other.m_StartingSlot){

    }

    MaterialHandle &MaterialHandle::operator=(MaterialHandle &&other) noexcept {
        if (this == &other){
            return *this;
        }
        m_Context = other.m_Context;
        m_Albedo = other.m_Albedo;
        m_Normal = other.m_Normal;
        m_Metallic = other.m_Metallic;
        m_Roughness = other.m_Roughness;
        m_Ao = other.m_Ao;
        m_StartingSlot = other.m_StartingSlot;
        return *this;
    }

    MaterialHandle::~MaterialHandle() {

    }

    void MaterialHandle::bind() {
        m_Albedo->bindShaderResource(m_StartingSlot);
        m_Normal->bindShaderResource(m_StartingSlot + 1);
        m_Metallic->bindShaderResource(m_StartingSlot + 2);
        m_Roughness->bindShaderResource(m_StartingSlot + 3);
        m_Ao->bindShaderResource(m_StartingSlot + 4);
    }

    void MaterialHandle::createTexture2D(const Resource<Image> & image, ResourceView<Texture2D> &texture) {
        Texture2DImageDesc desc;
        desc.image = createResourceView(image);
        texture = m_Context->createTexture2D(desc);
    }

}
