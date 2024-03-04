#pragma once

#include "Primitives.hpp"

namespace Syrius{

    class SR_API RenderCommand{
    public:
        RenderCommand() = default;

        virtual ~RenderCommand() = default;

        virtual void onResize(uint32_t width, uint32_t height) = 0;

        virtual void setProjectionFOV(float fov) = 0;

        virtual void setPlane(float near, float far) = 0;

        virtual MeshID createMesh(const MeshDesc& meshDesc) = 0;

        virtual void transformMesh(MeshID mesh, const glm::mat4& transform) = 0;

        virtual void removeMesh(MeshID mesh) = 0;

        virtual void updateCamera(const glm::mat4 &viewMat, const glm::vec3 &camPos) = 0;

        virtual MaterialID createMaterial(const Material& material) = 0;

        virtual void removeMaterial(MaterialID materialID) = 0;

        virtual LightID createLight(const Light& light) = 0;

        virtual void updateLight(LightID lightID, const Light& light) = 0;

        virtual void removeLight(LightID lightID) = 0;

        virtual void setCameraData(const glm::mat4& viewMat, const glm::vec3& camPos) = 0;

        virtual void meshSetMaterial(MeshID meshID, MaterialID materialID) = 0;
    };

}