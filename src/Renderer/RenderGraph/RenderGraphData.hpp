#pragma once

#include "Stores/ShaderStore.hpp"
#include "Stores/GeometryStore.hpp"

#include "Handles/TransformHandle.hpp"
#include "Handles/ProjectionHandle.hpp"
#include "Handles/CameraHandle.hpp"
#include "Handles/SamplerHandle.hpp"
#include "Handles/GBufferHandle.hpp"

namespace Syrius::Renderer {

    struct RenderGraphData {
        std::unordered_map<InstanceID, MeshID> instanceToMeshID;

        UP<ShaderStore> shaderStore;
        UP<GeometryStore> geometryStore;

        UP<TransformHandle> transformHandle;
        UP<ProjectionHandle> projectionHandle;
        UP<CameraHandle> cameraHandle;
        UP<SamplerHandle> samplerHandle;
        UP<GBufferHandle> gBufferHandle;

    };

}