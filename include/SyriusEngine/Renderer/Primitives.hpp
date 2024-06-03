#pragma once

#include "../Core/SyriusEngineInclude.hpp"

namespace Syrius{

    typedef UID MeshID;
    typedef UID MeshDataID;
    typedef UID MaterialID;
    typedef UID LightID;

    struct SR_API Vertex{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texCoord;
    };

    struct SR_API MeshDesc{
        std::vector<Vertex> vertices;
        std::vector<uint32> indices;
    };

    struct SR_API Light{
        glm::vec3 position;
        glm::vec3 color;
    };

}