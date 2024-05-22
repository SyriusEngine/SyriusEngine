#pragma once

#include "../../../../include/SyriusEngine/RenderLayer.hpp"
#include "../../../../include/SyriusEngine/Renderer/RenderCommand.hpp"

#include "../ShaderLibrary.hpp"
#include "RenderGraph.hpp"

#include "Passes/ProjectionPass.hpp"
#include "Passes/CameraDataPass.hpp"
#include "Passes/LFWRSamplerPass.hpp"
#include "Passes/GeometryPass.hpp"
#include "Passes/LightDataPass.hpp"
#include "Passes/GBufferPass.hpp"
#include "Passes/LightPass.hpp"

namespace Syrius{

    class RenderGraphLayer: public RenderLayer, RenderCommand {
    public:
        explicit RenderGraphLayer(Resource<ShaderLibrary>& shaderLibrary);

        ~RenderGraphLayer() override;

        void onAttach() override;

        void onDetach() override;

        void onUpdate() override;

        bool onEvent(const Event& event) override;

        void onRendererAttach(ResourceView<Context>& context) override;

        void onRendererDetach(ResourceView<Context>& context) override;

        void onRender(ResourceView<Context>& context) override;

        void onResize(uint32_t width, uint32_t height) override;

        void setProjectionFOV(float fov) override;

        void setPlane(float near, float far) override;

        MeshID createMesh(const MeshDesc& meshDesc) override;

        MeshID createMesh(MeshID meshID) override;

        void transformMesh(MeshID mesh, const glm::mat4& transform) override;

        void removeMesh(MeshID mesh) override;

        void updateCamera(const glm::mat4 &viewMat, const glm::vec3 &camPos) override;

        MaterialID createMaterial(const MaterialDesc& material) override;

        void meshSetMaterial(MeshID meshID, MaterialID materialID) override;

        void removeMaterial(MaterialID materialID) override;

        LightID createLight(const Light& light) override;

        void updateLight(LightID lightID, const Light& light) override;

        void removeLight(LightID lightID) override;

    private:
        Resource<ShaderLibrary>& m_ShaderLibrary;

        Resource<RenderGraph> m_RenderGraph;
    };
}