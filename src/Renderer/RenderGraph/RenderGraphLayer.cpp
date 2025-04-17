#include "RenderGraphLayer.hpp"

namespace Syrius::Renderer {

    void RenderGraphLayer::onRendererAttach(const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::onRendererDetach(const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::onRender(const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::onResize(u32 width, u32 height, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::createMesh(MeshID meshID, const Mesh &mesh, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::createInstance(InstanceID instanceID, MeshID meshID, const ResourceView<Context> &ctx) {  }


    void RenderGraphLayer::destroyMesh(MeshID meshID, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::destroyInstance(InstanceID instanceID) {

    }

    void RenderGraphLayer::setInstanceTransform(MeshID meshID, const Transform &transform, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::createCamera(CameraID cameraID, const Camera &camera, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::updateCamera(CameraID cameraID, const Camera &camera, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::destroyCamera(CameraID cameraID, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::createLight(LightID lightID, const Light &light, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::updateLight(LightID lightID, const Light &light, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::destroyLight(LightID lightID, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::createProjection(ProjectionID projectionID, const Projection &projection, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::updateProjection(ProjectionID projectionID, const Projection &projection, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::destroyProjection(ProjectionID projectionID, const ResourceView<Context> &ctx) {

    }


}
