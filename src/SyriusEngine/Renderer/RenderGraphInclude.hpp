#pragma once

#include "../../../include/SyriusEngine/Renderer/Primitives.hpp"
#include "../Core/InternalInclude.hpp"

namespace Syrius{

    typedef uint64 PassID;

    template<typename T>
    class PassIDGenerator{
    public:
        static PassID getID(){
            static PassID id = generateID();
            return id;
        }
    };

    class Pass;

    struct PassDesc{
        std::string name;
        PassID id;
        std::vector<PassID> dependencies;
        Pass* pAddress;
    };

    static float s_ScreenVertices[] = {
            // positions        // texture Coords
            -1.0f,  -1.0f,  0.0f, 0.0f,
            -1.0f,   1.0f,  0.0f, 1.0f,
            1.0f,  -1.0f,  1.0f, 0.0f,
            1.0f,   1.0f,  1.0f, 1.0f,
    };

    static uint32 s_ScreenIndices[] = {
            0, 1, 2,
            1, 2, 3
    };

    static float s_UnitCubeVertices[] = {
            // positions                normals              texture Coords   face ID
            -1.0f, -1.0f,   -1.0f,      0.0f,  0.0f, -1.0f,     0.0f, 0.0f,     5.0f,              // bottom-left
            1.0f,   1.0f,   -1.0f,      0.0f,  0.0f, -1.0f,     1.0f, 1.0f,     5.0f,              // top-right
            1.0f,   -1.0f,  -1.0f,      0.0f,  0.0f, -1.0f,     1.0f, 0.0f,     5.0f,              // bottom-right
            1.0f,   1.0f,   -1.0f,      0.0f,  0.0f, -1.0f,     1.0f, 1.0f,     5.0f,              // top-right
            -1.0f,  -1.0f,  -1.0f,      0.0f,  0.0f, -1.0f,     0.0f, 0.0f,     5.0f,              // bottom-left
            -1.0f,  1.0f,   -1.0f,      0.0f,  0.0f, -1.0f,     0.0f, 1.0f,     5.0f,              // top-left
            // front face
            -1.0f,  -1.0f,  1.0f,       0.0f,  0.0f,  1.0f,     0.0f, 0.0f,     4.0f,               // bottom-left
            1.0f,   -1.0f,  1.0f,       0.0f,  0.0f,  1.0f,     1.0f, 0.0f,     4.0f,               // bottom-right
            1.0f,   1.0f,   1.0f,       0.0f,  0.0f,  1.0f,     1.0f, 1.0f,     4.0f,               // top-right
            1.0f,   1.0f,   1.0f,       0.0f,  0.0f,  1.0f,     1.0f, 1.0f,     4.0f,               // top-right
            -1.0f,  1.0f,   1.0f,       0.0f,  0.0f,  1.0f,     0.0f, 1.0f,     4.0f,               // top-left
            -1.0f,  -1.0f,  1.0f,       0.0f,  0.0f,  1.0f,     0.0f, 0.0f,     4.0f,               // bottom-left
            // left face
            -1.0f,  1.0f,   1.0f,       -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,     1.0f,               // top-right
            -1.0f,  1.0f,   -1.0f,      -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,     1.0f,               // top-left
            -1.0f,  -1.0f,  -1.0f,      -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,     1.0f,               // bottom-left
            -1.0f,  -1.0f,  -1.0f,      -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,     1.0f,               // bottom-left
            -1.0f,  -1.0f,  1.0f,       -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,     1.0f,               // bottom-right
            -1.0f,  1.0f,   1.0f,       -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,     1.0f,               // top-right
            // right face
            1.0f,   1.0f,   1.0f,       1.0f,  0.0f,  0.0f,     1.0f, 0.0f,     0.0f,               // top-left
            1.0f,   -1.0f,  -1.0f,      1.0f,  0.0f,  0.0f,     0.0f, 1.0f,     0.0f,               // bottom-right
            1.0f,   1.0f,   -1.0f,      1.0f,  0.0f,  0.0f,     1.0f, 1.0f,     0.0f,               // top-right
            1.0f,   -1.0f,  -1.0f,      1.0f,  0.0f,  0.0f,     0.0f, 1.0f,     0.0f,               // bottom-right
            1.0f,   1.0f,   1.0f,       1.0f,  0.0f,  0.0f,     1.0f, 0.0f,     0.0f,               // top-left
            1.0f,   -1.0f,  1.0f,       1.0f,  0.0f,  0.0f,     0.0f, 0.0f,     0.0f,               // bottom-left
            // bottom face
            -1.0f,  -1.0f,  -1.0f,      0.0f, -1.0f,  0.0f,     0.0f, 1.0f,     3.0f,               // top-right
            1.0f,   -1.0f,  -1.0f,      0.0f, -1.0f,  0.0f,     1.0f, 1.0f,     3.0f,               // top-left
            1.0f,   -1.0f,  1.0f,       0.0f, -1.0f,  0.0f,     1.0f, 0.0f,     3.0f,               // bottom-left
            1.0f,   -1.0f,  1.0f,       0.0f, -1.0f,  0.0f,     1.0f, 0.0f,     3.0f,               // bottom-left
            -1.0f,  -1.0f,  1.0f,       0.0f, -1.0f,  0.0f,     0.0f, 0.0f,     3.0f,               // bottom-right
            -1.0f,  -1.0f,  -1.0f,      0.0f, -1.0f,  0.0f,     0.0f, 1.0f,     3.0f,               // top-right
            // top face
            -1.0f,  1.0f,   -1.0f,      0.0f,  1.0f,  0.0f,     0.0f, 1.0f,     2.0f,                // top-left
            1.0f,   1.0f ,  1.0f,       0.0f,  1.0f,  0.0f,     1.0f, 0.0f,     2.0f,               // bottom-right
            1.0f,   1.0f,   -1.0f,      0.0f,  1.0f,  0.0f,     1.0f, 1.0f,     2.0f,               // top-right
            1.0f,   1.0f,   1.0f,       0.0f,  1.0f,  0.0f,     1.0f, 0.0f,     2.0f,               // bottom-right
            -1.0f,  1.0f,   -1.0f,      0.0f,  1.0f,  0.0f,     0.0f, 1.0f,     2.0f,               // top-left
            -1.0f,  1.0f,   1.0f,       0.0f,  1.0f,  0.0f,     0.0f, 0.0f,     2.0f,               // bottom-left
    };

}