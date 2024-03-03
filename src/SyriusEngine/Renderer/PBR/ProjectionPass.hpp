#pragma once

#include "../Pass.hpp"

namespace Syrius{

    struct ProjectionDesc{
        float windowWidth = SR_DEFAULT_WIDTH;
        float windowHeight = SR_DEFAULT_HEIGHT;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
        float fov = 45.0f;
    };

    struct ProjectionPassDesc{
        ProjectionDesc projection;
        uint32 slot;
        std::string bufferName;
    };

    struct ProjectionData{
        glm::mat4 perspective = glm::mat4(1.0f);
        glm::mat4 orthographic = glm::mat4(1.0f);
    };

    class ProjectionPass: public Pass{
    public:
        ProjectionPass(ResourceView<Context>& context, ProjectionPassDesc& desc);

        ~ProjectionPass() override;

        void execute(PassMap& passMap) override;

        void onResize(uint32 width, uint32 height);

        void setFOV(float fov);

        void setPlane(float nearPlane, float farPlane);

    private:
        void update();

    private:
        uint32 m_Slot;

        ProjectionDesc m_Projection;
        ProjectionData m_Data;

        ResourceView<ConstantBuffer> m_ProjectionBuffer;

    };

}