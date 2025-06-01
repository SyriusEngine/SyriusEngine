#pragma once

#include "../IRenderGraphData.hpp"
#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    class CameraHandle : public IRenderGraphData {
    public:
        CameraHandle(const ResourceView<Context>& ctx, RenderGraphContainer* container,
                     const SP<DispatcherManager>& dispatcherManager);

        ~CameraHandle() override = default;

        void setCamera(CameraID cameraID, const SP<Camera>& camera) const;

        void bind(u32 slot) const;

    private:
        ResourceView<ConstantBuffer> m_CameraBuffer;
    };
} // namespace Syrius::Renderer
