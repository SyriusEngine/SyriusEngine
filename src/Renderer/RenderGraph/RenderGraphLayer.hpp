#pragma once

#include <SyriusEngine/Renderer/RenderLayer.hpp>

namespace Syrius {

    class RenderGraphLayer: public IRenderLayer {
    public:
        RenderGraphLayer() = default;
        
        ~RenderGraphLayer() override = default;

        void onRendererAttach(const ResourceView<Context>& ctx) override;

        void onRendererDetach(const ResourceView<Context>& ctx) override;

        void onRender(const ResourceView<Context>& ctx) override;

        void onResize(u32 width, u32 height, const ResourceView<Context>& ctx) override;

        void createMesh(MeshID meshID, const Mesh& mesh, const ResourceView<Context>& ctx) override;

        void destroyMesh(MeshID meshID, const ResourceView<Context>& ctx) override;

        void setMeshTransform(MeshID meshID, const Transform& transform, const ResourceView<Context>& ctx) override;

        void createCamera(CameraID cameraID, const Camera& camera, const ResourceView<Context>& ctx) override;

        void updateCamera(CameraID cameraID, const Camera& camera, const ResourceView<Context>& ctx) override;

        void destroyCamera(CameraID cameraID, const ResourceView<Context>& ctx) override;

        void createLight(LightID lightID, const Light& light, const ResourceView<Context>& ctx) override;

        void updateLight(LightID lightID, const Light& light, const ResourceView<Context>& ctx) override;

        void destroyLight(LightID lightID, const ResourceView<Context>& ctx) override;

        void createProjection(ProjectionID projectionID, const Projection& projection, const ResourceView<Context>& ctx) override;

        void updateProjection(ProjectionID projectionID, const Projection& projection, const ResourceView<Context>& ctx) override;

        void destroyProjection(ProjectionID projectionID, const ResourceView<Context>& ctx) override;


    };
}