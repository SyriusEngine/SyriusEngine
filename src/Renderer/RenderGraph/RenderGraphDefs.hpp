#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

#include <string>

namespace Syrius::Renderer {

    struct InstanceData {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 inverseTranspose = glm::mat4(1.0f);
    };

    typedef enum SR_RENDER_NODE {
        SR_NODE_NONE = 0,
        SR_NODE_TRANSFORM_DATA = 1,
        SR_NODE_PROJECTION_DATA = 2,
        SR_NODE_CAMERA_DATA = 3,
        SR_NODE_LIGHT_DATA= 4,
        SR_NODE_GBUFFER = 5,
        SR_NODE_GEOMETRY_DATA = 6,


        SR_NODE_DRAW_GEOMETRY = 100,
        SR_NODE_DRAW_LIGHTS = 101,

    }SR_RENDER_NODE;

    std::string renderNodeToString(SR_RENDER_NODE render_node);

    static std::string s_GEOMETRY_PASS_SHADER = "Basic";
}