#pragma once

#include "Stores/ShaderStore.hpp"
#include "Stores/GeometryStore.hpp"

#include "Handles/TransformHandle.hpp"
#include "Handles/ProjectionHandle.hpp"
#include "Handles/CameraHandle.hpp"

namespace Syrius::Renderer {

    struct RenderGraphData {
        std::unordered_map<InstanceID, MeshID> instanceToMeshID;

        UP<ShaderStore> shaderStore;
        UP<GeometryStore> geometryStore;

        UP<TransformHandle> transformHandle;
        UP<ProjectionHandle> projectionHandle;
        UP<CameraHandle> cameraHandle;
    };

}