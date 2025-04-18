#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include <glm/glm.hpp>

typedef enum SR_RENDERER_SYSTEM {
    SR_RENDERER_SYSTEM_NONE = 0,
    SR_RENDERER_SYSTEM_DEFAULT = 1,
} SR_RENDERER_SYSTEM;

namespace Syrius{

    using MeshID = UID;
    using InstanceID = UID;
    using TransformID = UID;
    using MaterialID = UID;
    using LightID = UID;
    using CameraID = UID;
    using ProjectionID = UID;

    constexpr ProjectionID SR_DEFAULT_PROJECTION = 0;

    struct SR_ENGINE_API Vertex{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texCoord;
    };

    struct SR_ENGINE_API Mesh{
        std::vector<Vertex> vertices;
        std::vector<u32> indices;
    };

    struct SR_ENGINE_API Transform {
        glm::mat4 model = glm::mat4(1.0f);
    };

    struct SR_ENGINE_API Camera{
        glm::mat4 view      = glm::mat4(1.0f);
        glm::vec4 position  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    struct SR_ENGINE_API Light{
        glm::vec3 position;
        glm::vec3 color;
    };

    struct SR_ENGINE_API Projection{
        float windowWidth = SR_DEFAULT_WIDTH;
        float windowHeight = SR_DEFAULT_HEIGHT;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
        float fov = 45.0f;
    };
}

