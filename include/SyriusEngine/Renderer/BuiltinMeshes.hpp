#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

namespace Syrius {

    SR_ENGINE_API Mesh createTriangle();

    SR_ENGINE_API Mesh createRectangle();

    SR_ENGINE_API Mesh createPyramid();

    SR_ENGINE_API Mesh createCube();

    SR_ENGINE_API Mesh createSphere(u32 rings = 32, u32 sectors = 32);

    SR_ENGINE_API Mesh createCone(u32 sectors = 32);

    SR_ENGINE_API Mesh createTorus(u32 rings = 32, u32 sectors = 32);

    SR_ENGINE_API Mesh createCylinder(u32 sectors = 32);

}