#include "BuiltIns.hpp"

namespace Syrius{

    void calculateTangents(MeshDesc& mesh){
        for (int i = 0; i < mesh.indices.size(); i += 3) {
            auto& v0 = mesh.vertices[mesh.indices[i + 0]];
            auto& v1 = mesh.vertices[mesh.indices[i + 1]];
            auto& v2 = mesh.vertices[mesh.indices[i + 2]];

            auto edge1 = v1.position - v0.position;
            auto edge2 = v2.position - v0.position;

            auto deltaUV1 = v1.texCoord - v0.texCoord;
            auto deltaUV2 = v2.texCoord - v0.texCoord;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            auto tangent = glm::vec3();
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            v0.tangent = tangent;
            v1.tangent = tangent;
            v2.tangent = tangent;
        }
    }

    void createCube(MeshDesc& cube){
        cube.vertices = {
                /// front
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// left
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// bottom
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// back
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// right
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// top
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        };
        cube.indices = {
                0, 3, 2,
                0, 2, 1,

                4, 7, 6,
                4, 6, 5,

                8, 11, 10,
                8, 10, 9,

                12, 15, 14,
                12, 14, 13,

                16, 19, 18,
                16, 18, 17,

                20, 23, 22,
                20, 22, 21
        };
        calculateTangents(cube);
    }

}