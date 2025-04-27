#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Utils/EngineLimits.hpp>
#include <SyriusEngine/Renderer/Material.hpp>

#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    class MaterialHandle {
    public:
        MaterialHandle(MaterialID materialID, const ResourceView<Context>& ctx, const Material& material);

        ~MaterialHandle() = default;

        MaterialHandle(const MaterialHandle& other) = delete;
        MaterialHandle(MaterialHandle&& other) noexcept;

        MaterialHandle& operator=(const MaterialHandle& other) = delete;
        MaterialHandle& operator=(MaterialHandle&& other) noexcept;

        void bind(u32 startSlot) const;

    private:
        static void loadTexture(const ResourceView<Context> &ctx, const UP<Image>& image, ResourceView<Texture2D>& textureView);

    private:
        MaterialID m_MaterialID = 0;
        ResourceView<Texture2D> m_Albedo;
        ResourceView<Texture2D> m_Normal;
        ResourceView<Texture2D> m_MRAO;
    };
}