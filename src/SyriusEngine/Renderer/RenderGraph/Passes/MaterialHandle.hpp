#pragma once

#include "../../RendererInclude.hpp"

namespace Syrius{

    class MaterialHandle{
    public:
        MaterialHandle(ResourceView<Context>& context, const MaterialDesc& desc, uint32 slot);

        ~MaterialHandle();

        MaterialHandle(const MaterialHandle& other) = delete;

        MaterialHandle(MaterialHandle&& other) noexcept;

        MaterialHandle& operator=(const MaterialHandle& other) = delete;

        MaterialHandle& operator=(MaterialHandle&& other) noexcept;

        void bind();

    private:

        void validateImages(const MaterialDesc& desc);

    private:
        ResourceView<Context>& m_Context;

        ResourceView<Texture2D> m_Material;

        uint32 m_Slot;
    };

}