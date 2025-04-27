#pragma once

#include "Stores/ShaderStore.hpp"
#include "Stores/GeometryStore.hpp"
#include "Stores/MaterialStore.hpp"

#include "Handles/TransformHandle.hpp"
#include "Handles/ProjectionHandle.hpp"
#include "Handles/CameraHandle.hpp"
#include "Handles/SamplerHandle.hpp"
#include "Handles/GBufferHandle.hpp"

#include "Other/ScreenQuad.hpp"

namespace Syrius::Renderer {

    struct RenderGraphData {
        std::unordered_map<InstanceID, MeshID> instanceToMeshID;
        std::unordered_map<MaterialID, std::vector<MeshID>> materialToMesh;
        UP<ScreenQuad> lightPassQuad;

        UP<ShaderStore> shaderStore;
        UP<GeometryStore> geometryStore;
        UP<MaterialStore> materialStore;

        UP<TransformHandle> transformHandle;
        UP<ProjectionHandle> projectionHandle;
        UP<CameraHandle> cameraHandle;
        UP<SamplerHandle> samplerHandle;
        UP<GBufferHandle> gBufferHandle;
    };

}