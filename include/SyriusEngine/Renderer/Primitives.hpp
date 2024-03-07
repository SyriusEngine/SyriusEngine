#pragma once

#include "../Core/SyriusEngineInclude.hpp"

namespace Syrius{

    typedef UID MeshID;
    typedef UID MaterialID;
    typedef UID LightID;
    typedef UID CameraID;

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

    struct SR_API MaterialDesc{
        Resource<Image> albedo = nullptr;
        Resource<Image> normal = nullptr;
        Resource<Image> metallic = nullptr;
        Resource<Image> roughness = nullptr;
        Resource<Image> ao = nullptr;
    };

    struct SR_API Light{
        glm::vec3 position;
        glm::vec3 color;
    };

}