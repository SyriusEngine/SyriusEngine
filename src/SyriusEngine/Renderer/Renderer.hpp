#pragma once

#include "../../../include/SyriusEngine/Renderer/RenderCommand.hpp"
#include "../LayerStack.hpp"
#include "../../../include/SyriusEngine/Utils/Worker.hpp"
#include "ShaderLibrary.hpp"

#include "RenderGraph/RenderGraphLayer.hpp"

namespace Syrius{

    struct RendererDesc{
        SR_SUPPORTED_API graphicsAPI;
        std::string shaderLibraryPath;
        bool vSync = false;
        float clearColor[3];
        bool enableExperimentalSRSLShaderCompiler = false;
    };

    class Renderer: public RenderCommand{
    public:
        Renderer(const RendererDesc& desc, LayerStack& layerStack, const Resource<SyriusWindow>& window);

        ~Renderer() override;

        void render();

        void swapBuffers();

        void pushRenderLayer(Resource<RenderLayer> layer);

        void onResize(uint32_t width, uint32_t height) override;

        void setProjectionFOV(float fov) override;

        void setPlane(float near, float far) override;

        MeshID createMesh(const MeshDesc& meshDesc) override;

        MeshID createMesh(MeshID meshID) override;

        void transformMesh(MeshID mesh, const glm::mat4& transform) override;

        void removeMesh(MeshID mesh) override;

        void updateCamera(const glm::mat4 &viewMat, const glm::vec3 &camPos) override;

        MaterialID createMaterial(const MaterialDesc& material) override;

        void removeMaterial(MaterialID materialID) override;

        LightID createLight(const Light& light) override;

        void updateLight(LightID lightID, const Light& light) override;

        void removeLight(LightID lightID) override;

        void meshSetMaterial(MeshID meshID, MaterialID materialID) override;

        void setSkyBox(const Resource<Image>& radianceMap) override;

    private:
        LayerStack& m_LayerStack;

        const Resource<SyriusWindow>& m_Window;
        ResourceView<Context> m_Context;

        Resource<ShaderLibrary> m_ShaderLibrary;

        Worker m_RenderThread;

        ResourceView<RenderGraphLayer> m_ActiveLayer;
    };

}