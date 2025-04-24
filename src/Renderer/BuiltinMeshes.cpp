#include <SyriusEngine/Renderer/BuiltinMeshes.hpp>

namespace Syrius {

    void calculateTangents(Mesh& mesh){
        for (int i = 0; i < mesh.indices.size(); i += 3) {
            auto& v0 = mesh.vertices[mesh.indices[i + 0]];
            auto& v1 = mesh.vertices[mesh.indices[i + 1]];
            auto& v2 = mesh.vertices[mesh.indices[i + 2]];

            auto edge1 = v1.position - v0.position;
            auto edge2 = v2.position - v0.position;

            auto deltaUV1 = v1.texCoord - v0.texCoord;
            auto deltaUV2 = v2.texCoord - v0.texCoord;

            const float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

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

    Mesh createTriangle(){
        Mesh triangle;
        triangle.vertices = {
                {glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.5f, 1.0f)}
        };
        triangle.indices = {0, 1, 2};
        calculateTangents(triangle);
        return triangle;
    }

    Mesh createRectangle(){
        Mesh rectangle;
        rectangle.vertices = {
                {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
        };
        rectangle.indices = {
                0, 1, 2,
                0, 2, 3,
        };
        calculateTangents(rectangle);
        return rectangle;
    }

    Mesh createPyramid(){
        Mesh pyramid;
        pyramid.vertices = {
                { { 0.0f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
                { {  0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },
                { {  -0.5f,  -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },
                { {  -0.5f,  -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
                { {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
        };

        pyramid.indices = {
                0, 1, 2,
                0, 2, 3,
                0, 3, 4,
                0, 4, 1,
                1, 4, 3,
                1, 3, 2
        };
        calculateTangents(pyramid);
        return pyramid;
    }

    Mesh createCube(){
        Mesh cube;
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
        return cube;
    }

    Mesh createSphere(u32 rings, u32 sectors){
        SR_PRECONDITION(rings > 2, "Rings should be at least 2 (is {})", rings);
        SR_PRECONDITION(sectors > 2, "Sectors should be at least 2 (is {}", sectors);
        
        Mesh sphere;
        float R = 1.0f / static_cast<float>(rings - 1);
        float S = 1.0f / static_cast<float>(sectors - 1);
        u32 r = 0;
        u32 s = 0;

        for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                float y = glm::sin(-M_PI_2 + M_PI * r * R );
                float x = glm::cos(2 * M_PI * s * S) * glm::sin(M_PI * r * R );
                float z = glm::sin(2 * M_PI * s * S) * glm::sin(M_PI * r * R );

                auto position = glm::vec3(x, y, z);
                auto normal = glm::normalize(position);
                auto tangent = glm::vec3(0.0f, 1.0f, 0.0f);
                auto texCoords = glm::vec2(s*S, r*R);

                sphere.vertices.push_back({position, normal, tangent, texCoords});
            }

        for(r = 0; r < rings - 1; r++) {
            for(s = 0; s < sectors-1; s++) {
                sphere.indices.push_back(r * sectors + s);
                sphere.indices.push_back(r * sectors + (s + 1));
                sphere.indices.push_back((r + 1) * sectors + (s + 1));

                sphere.indices.push_back(r * sectors + s);
                sphere.indices.push_back((r + 1) * sectors + (s + 1));
                sphere.indices.push_back((r + 1) * sectors + s);
            }   
        }
        calculateTangents(sphere);
        return sphere;
    }

    Mesh createCone(u32 sectors){
        SR_PRECONDITION(sectors > 2, "Sectors should be at least 2 (is {}", sectors);

        Mesh cone;        
        float S = 1.0f/static_cast<float>(sectors - 1);
        u32 s;

        cone.vertices.push_back({{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
        for(s = 0; s < sectors; s++) {
            float const x = cos(2 * M_PI * s * S);
            float const z = sin(2 * M_PI * s * S);

            cone.vertices.push_back({{x, -0.5f, z}, {x, -0.5f, z}, {0.0f, 1.0f, 0.0f}, {s*S, 1.0f}});
        }

        for(s = 0; s < sectors-1; s++) {
            cone.indices.push_back(0);
            cone.indices.push_back(s+1);
            cone.indices.push_back(s+2);
        }
        cone.indices.push_back(0);
        cone.indices.push_back(sectors);
        cone.indices.push_back(1);

        calculateTangents(cone);
        return cone;
    }

    Mesh createTorus(u32 rings, u32 sectors){
        SR_PRECONDITION(rings > 2, "Rings should be at least 2 (is {})", rings);
        SR_PRECONDITION(sectors > 2, "Sectors should be at least 2 (is {}", sectors);
        
        float R = 1.0f/static_cast<float>(rings-1);
        float S = 1.0f/static_cast<float>(sectors-1);
        u32 r = 0;
        u32 s = 0;
        Mesh torus;

        for(r = 0; r < rings; r++) {
            for(s = 0; s < sectors; s++) {
                float y = (1 + 0.5f * glm::cos(2 * M_PI * r * R)) * glm::sin(-M_PI_2 + M_PI * s * S);
                float x = (1 + 0.5f * glm::cos(2 * M_PI * r * R)) * glm::cos(2 * M_PI * s * S);
                float z = 0.5f * glm::sin(2 * M_PI * r * R);

                torus.vertices.push_back({{x, y, z}, {x, y, z}, {0.0f, 1.0f, 0.0f}, {s * S, r * R}});
            }
        }

        for (r = 0; r < rings-1; r++) {
            for (s = 0; s < sectors-1; s++) {
                torus.indices.push_back(r * sectors + s);
                torus.indices.push_back(r * sectors + (s + 1));
                torus.indices.push_back((r + 1) * sectors + (s + 1));

                torus.indices.push_back(r * sectors + s);
                torus.indices.push_back((r + 1) * sectors + (s + 1));
                torus.indices.push_back((r + 1) * sectors + s);
            }
        }
        calculateTangents(torus);
        return torus;
    }

    Mesh createCylinder(u32 sectors) {
        SR_PRECONDITION(sectors > 2, "Sectors should be at least 2 (is {}", sectors);

        float S = 1.0f / static_cast<float> (sectors - 1);
        u32 s = 0;
        Mesh cylinder;

        cylinder.vertices.push_back({{0.0f, 0.5f, 0.0f},
                                     {0.0f, 1.0f, 0.0f},
                                     {0.0f, 0.0f, 0.0f},
                                     {0.0f, 0.0f}});
        for (s = 0; s < sectors; s++) {
            float x = cos(2 * M_PI * s * S);
            float z = sin(2 * M_PI * s * S);

            cylinder.vertices.push_back({{x,     0.5f, z},
                                         {0.0f,  1.0f, 0.0f},
                                         {0.0f,  0.0f, 0.0f},
                                         {s * S, 1.0f}});
            cylinder.vertices.push_back({{x,     -0.5f, z},
                                         {0.0f,  1.0f,  0.0f},
                                         {0.0f,  0.0f,  0.0f},
                                         {s * S, 0.0f}});
        }

        for (s = 0; s < sectors - 1; s++) {
            cylinder.indices.push_back(0);
            cylinder.indices.push_back(s * 2 + 2);
            cylinder.indices.push_back(s * 2 + 4);
        }
        cylinder.indices.push_back(0);
        cylinder.indices.push_back(sectors * 2);
        cylinder.indices.push_back(2);

        for (s = 0; s < sectors - 1; s++) {
            cylinder.indices.push_back(1);
            cylinder.indices.push_back(s * 2 + 3);
            cylinder.indices.push_back(s * 2 + 5);
        }
        cylinder.indices.push_back(1);
        cylinder.indices.push_back(3);
        cylinder.indices.push_back(sectors * 2 + 1);

        for (s = 0; s < sectors - 1; s++) {
            cylinder.indices.push_back(s * 2 + 2);
            cylinder.indices.push_back(s * 2 + 3);
            cylinder.indices.push_back(s * 2 + 4);

            cylinder.indices.push_back(s * 2 + 4);
            cylinder.indices.push_back(s * 2 + 3);
            cylinder.indices.push_back(s * 2 + 5);

        }
        cylinder.indices.push_back(sectors * 2 - 2);
        cylinder.indices.push_back(sectors * 2 - 1);
        cylinder.indices.push_back(2);

        calculateTangents(cylinder);
        return cylinder;
    }

}
