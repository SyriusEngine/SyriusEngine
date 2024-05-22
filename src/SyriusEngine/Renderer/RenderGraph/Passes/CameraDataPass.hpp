#pragma once

#include "../Pass.hpp"

namespace Syrius{

    struct CameraDataPassDesc{
        uint32 slot;
        std::string bufferName;
    };

    struct CameraData{
        glm::mat4 view      = glm::mat4(1.0f);
        glm::vec4 position  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    class CameraDataPass : public Pass{
    public:
        CameraDataPass(ResourceView<Context>& context, CameraDataPassDesc& desc);

        ~CameraDataPass() override;

        void execute(PassMap& passMap) override;

        void setCameraData(const glm::mat4& viewMat, const glm::vec3& camPos);

    private:
        uint32 m_Slot;

        CameraData m_Data;

        ResourceView<ConstantBuffer> m_CameraBuffer;
    };

}