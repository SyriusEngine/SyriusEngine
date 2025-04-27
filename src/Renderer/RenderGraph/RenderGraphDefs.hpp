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
        SR_NODE_GEOMETRY_DATA = 6,
        SR_NODE_MATERIAL_DATA = 7,
        SR_NODE_SAMPLER_DATA = 8,

        SR_NODE_CLEAR_GBUFFER = 100,

        SR_NODE_DRAW_GEOMETRY = 1000,
        SR_NODE_DRAW_LIGHTS = 1001,
        SR_NODE_DRAW_GBUFFER = 1002,

    }SR_RENDER_NODE;

    std::string renderNodeToString(SR_RENDER_NODE render_node);

    static std::string s_GEOMETRY_PASS_SHADER = "GeometryPass";
    static std::string s_LIGHT_PASS_SHADER = "LightPass";
}