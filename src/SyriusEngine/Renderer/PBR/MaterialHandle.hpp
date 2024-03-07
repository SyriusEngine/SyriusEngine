#pragma once

#include "../RenderGraphInclude.hpp"

namespace Syrius{

    class MaterialHandle{
    public:
        MaterialHandle(ResourceView<Context>& context, const MaterialDesc& desc, uint32 startingSlot);

        ~MaterialHandle();

        MaterialHandle(const MaterialHandle& other) = delete;

        MaterialHandle(MaterialHandle&& other) noexcept;

        MaterialHandle& operator=(const MaterialHandle& other) = delete;

        MaterialHandle& operator=(MaterialHandle&& other) noexcept;

        void bind();

    private:

        void createTexture2D(const Resource<Image>& image, ResourceView<Texture2D>& texture);

    private:
        ResourceView<Context>& m_Context;

        ResourceView<Texture2D> m_Albedo;
        ResourceView<Texture2D> m_Normal;
        ResourceView<Texture2D> m_Metallic;
        ResourceView<Texture2D> m_Roughness;
        ResourceView<Texture2D> m_Ao;

        uint32 m_StartingSlot;
    };

}