#pragma once

#include "../../../include/SyriusEngine/RenderLayer.hpp"
#include "../../../include/SyriusEngine/Renderer/RenderCommand.hpp"

#include "../ShaderLibrary.hpp"
#include "../RenderGraph.hpp"

#include "ProjectionPass.hpp"
#include "CameraDataPass.hpp"
#include "LFWRSamplerPass.hpp"
#include "GeometryPass.hpp"
#include "LightDataPass.hpp"
#include "GBufferPass.hpp"
#include "LightPass.hpp"

namespace Syrius{

    class PBRRenderLayer: public RenderLayer, RenderCommand {
    public:
        explicit PBRRenderLayer(Resource<ShaderLibrary>& shaderLibrary);

        ~PBRRenderLayer() override;

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

        ProjectionPass* m_ProjectionPass;
        CameraDataPass* m_CameraDataPass;
        GeometryPass* m_GeometryPass;
        LightDataPass* m_LightDataPass;
    };
}