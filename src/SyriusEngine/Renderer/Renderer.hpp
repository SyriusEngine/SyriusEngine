#pragma once

#include "../../../include/SyriusEngine/Renderer/RenderCommand.hpp"
#include "../LayerStack.hpp"
#include "../Utils/Worker.hpp"
#include "ShaderLibrary.hpp"

#include "PBR/PBRRenderLayer.hpp"

namespace Syrius{

    struct RendererDesc{
        SR_SUPPORTED_API graphicsAPI;
        std::string shaderLibraryPath;
        bool vSync = true;
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

        void transformMesh(MeshID mesh, const glm::mat4& transform) override;

        void removeMesh(MeshID mesh) override;

    private:
        LayerStack& m_LayerStack;

        const Resource<SyriusWindow>& m_Window;
        ResourceView<Context> m_Context;

        Resource<ShaderLibrary> m_ShaderLibrary;

        Worker m_RenderThread;

        ResourceView<PBRRenderLayer> m_PBRLayer;
    };

}