#pragma once

#include "Renderer/RenderCommand.hpp"

namespace Syrius{

    SR_API void createTriangle(MeshDesc& triangle);

    SR_API void createRectangle(MeshDesc& rectangle);

    SR_API void createPyramid(MeshDesc& pyramid);

    SR_API void createCube(MeshDesc& cube);

    SR_API void createSphere(MeshDesc& sphere, uint32 rings = 32, uint32 sectors = 32);

    SR_API void createCone(MeshDesc& cone, uint32 sectors = 32);

    SR_API void createTorus(MeshDesc& torus, uint32 rings = 32, uint32 sectors = 32);

    SR_API void createCylinder(MeshDesc& cylinder, uint32 sectors = 32);

}