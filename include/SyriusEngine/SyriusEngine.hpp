#pragma once

#include "Core/SyriusEngineInclude.hpp"

#include "Renderer/Primitives.hpp"
#include "Renderer/RenderCommand.hpp"

#include "Utils/Worker.hpp"

#include "Layer.hpp"
#include "RenderLayer.hpp"
#include "BuiltIns.hpp"
#include "EngineConfiguration.hpp"

namespace Syrius{

    class SR_API SyriusEngine{
    public:
        explicit SyriusEngine(const EngineConfiguration& config);

        virtual ~SyriusEngine();

        virtual void run() = 0;

        virtual void pushLayer(Resource<Layer> layer) = 0;

        virtual void pushRenderLayer(Resource<RenderLayer> layer) = 0;

        virtual void shutdown() = 0;

        [[nodiscard]] inline const Resource<SyriusWindow>& getWindow() const { return m_Window; }

        [[nodiscard]] inline const ResourceView<RenderCommand>& getRenderCommand() const { return m_RenderCommand; }



    protected:
        Resource<SyriusWindow> m_Window;
        EngineConfiguration m_Config;

        ResourceView<RenderCommand> m_RenderCommand;

    private:
        static uint64 m_InstanceCount;
    };

    SR_API Resource<SyriusEngine> createEngine(const EngineConfiguration& config);

    SR_API void setDebugMessageHandler(HandleDebugMessageFunc cb);

}