#include "RenderGraphDefs.hpp"

namespace Syrius::Renderer{

    std::string renderNodeToString(SR_RENDER_NODE render_node) {
        switch (render_node) {
            case SR_NODE_NONE:              return "SR_NODE_NONE";
            case SR_NODE_TRANSFORM_DATA:    return "SR_NODE_TRANSFORM_DATA";
            case SR_NODE_PROJECTION_DATA:   return "SR_NODE_PROJECTION_DATA";
            case SR_NODE_CAMERA_DATA:       return "SR_NODE_CAMERA_DATA";
            case SR_NODE_LIGHT_DATA:        return "SR_NODE_LIGHT_DATA";
            case SR_NODE_GEOMETRY_DATA:     return "SR_NODE_GEOMETRY_DATA";
            case SR_NODE_MATERIAL_DATA:     return "SR_NODE_MATERIAL_DATA";
            case SR_NODE_SAMPLER_DATA:      return "SR_NODE_SAMPLER_DATA";

            case SR_NODE_CLEAR_GBUFFER:     return "SR_NODE_CLEAR_GBUFFER";

            case SR_NODE_DRAW_GEOMETRY:     return "SR_NODE_DRAW_GEOMETRY";
            case SR_NODE_DRAW_LIGHTS:       return "SR_NODE_DRAW_LIGHTS";
            case SR_NODE_DRAW_GBUFFER:      return "SR_NODE_DRAW_GBUFFER";
            default: return "";
        }
    }

}
