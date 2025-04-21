#pragma once

#include <SyriusEngine/Renderer/RenderLayer.hpp>

#include "RenderGraphData.hpp"
#include "RenderGraph.hpp"

namespace Syrius::Renderer {

    class RenderGraphLayer: public IRenderLayer {
    public:
        explicit RenderGraphLayer(const fs::path& shaderPath);
        
        ~RenderGraphLayer() override = default;

        void onRendererAttach(const ResourceView<Context>& ctx) override;

        void onRendererDetach(const ResourceView<Context>& ctx) override;

        void onRender(const ResourceView<Context>& ctx) override;

        void onResize(u32 width, u32 height, const ResourceView<Context>& ctx) override;

        void createMesh(MeshID meshID, const Mesh& mesh, const ResourceView<Context>& ctx) override;

        void createInstance(InstanceID instanceID, MeshID meshID, const ResourceView<Context> &ctx) override;

        void destroyMesh(MeshID meshID, const ResourceView<Context>& ctx) override;

        void destroyInstance(InstanceID instanceID, const ResourceView<Context>& ctx) override;

        void setInstanceTransform(InstanceID instanceID, const Transform& transform, const ResourceView<Context>& ctx) override;

        void createCamera(CameraID cameraID, const Camera& camera, const ResourceView<Context>& ctx) override;

        void updateCamera(CameraID cameraID, const Camera& camera, const ResourceView<Context>& ctx) override;

        void destroyCamera(CameraID cameraID, const ResourceView<Context>& ctx) override;

        void createLight(LightID lightID, const Light& light, const ResourceView<Context>& ctx) override;

        void updateLight(LightID lightID, const Light& light, const ResourceView<Context>& ctx) override;

        void destroyLight(LightID lightID, const ResourceView<Context>& ctx) override;

        void createProjection(ProjectionID projectionID, const Projection& projection, const ResourceView<Context>& ctx) override;

        void updateProjection(ProjectionID projectionID, const Projection& projection, const ResourceView<Context>& ctx) override;

        void destroyProjection(ProjectionID projectionID, const ResourceView<Context>& ctx) override;

    private:

        void createPNRRenderGraph();

    private:
        RenderGraphData m_RenderGraphData;
        RenderGraph m_RenderGraph;
    };
}