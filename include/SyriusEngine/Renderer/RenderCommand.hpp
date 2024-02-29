#pragma once

#include "Primitives.hpp"

namespace Syrius{

    class SR_API RenderCommand{
    public:
        RenderCommand() = default;

        virtual ~RenderCommand() = default;

        virtual MeshID createMesh(const MeshDesc& meshDesc) = 0;

        virtual void transformMesh(MeshID mesh, const glm::mat4& transform) = 0;

        virtual void removeMesh(MeshID mesh) = 0;
    };

}