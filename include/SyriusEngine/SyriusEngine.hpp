#pragma once

#include "Core/SyriusEngineInclude.hpp"

#include "Layer.hpp"
#include "RenderLayer.hpp"

namespace Syrius{

    class SR_API SyriusEngine{
    public:
        explicit SyriusEngine(const std::string& configFile);

        virtual ~SyriusEngine();

        virtual void run() = 0;

        virtual void pushLayer(Resource<Layer> layer) = 0;

        virtual void pushRenderLayer(Resource<RenderLayer> layer) = 0;

        [[nodiscard]] inline const Resource<SyriusWindow>& getWindow() const { return m_Window; }

    protected:
        Resource<SyriusWindow> m_Window;
        const std::string m_ConfigFile;

    private:
        static uint64 m_InstanceCount;
    };

    SR_API Resource<SyriusEngine> createEngine(const std::string& configFile = "config.ini");

}