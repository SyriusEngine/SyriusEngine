#pragma once

#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    struct ProjectionData {
        glm::mat4 projection = glm::mat4(1.0f);
    };

    class ProjectionHandle {
    public:
        explicit ProjectionHandle(const ResourceView<Context>& ctx);

        ~ProjectionHandle() = default;

        void updateProjection(ProjectionID projectionID, const Projection& projection) const;

        void bind(u32 slot) const;

    private:
        ResourceView<ConstantBuffer> m_ProjectionBuffer;
    };

}