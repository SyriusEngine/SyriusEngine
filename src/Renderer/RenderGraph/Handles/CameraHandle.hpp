#pragma once

#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    class CameraHandle {
    public:
        explicit CameraHandle(const ResourceView<Context>& ctx);

        ~CameraHandle() = default;

        void setCamera(CameraID cameraID, const Camera& camera) const;

        void bind(u32 slot) const;

    private:
        ResourceView<ConstantBuffer> m_CameraBuffer;
    };
}