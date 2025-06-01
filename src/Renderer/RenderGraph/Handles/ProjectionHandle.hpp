#pragma once

#include "../RenderGraphDefs.hpp"
#include "../IRenderGraphData.hpp"

namespace Syrius::Renderer {

    struct ProjectionData {
        glm::mat4 projection = glm::mat4(1.0f);
    };

    class ProjectionHandle: public IRenderGraphData {
    public:
        ProjectionHandle(const ResourceView<Context>& ctx, RenderGraphContainer* container, const SP<DispatcherManager>& dispatcherManager);

        ~ProjectionHandle() override = default;

        void updateProjection(ProjectionID projectionID, const SP<Projection>& projection) const;

        void bind(u32 slot) const;

    private:
        ResourceView<ConstantBuffer> m_ProjectionBuffer;
    };

}