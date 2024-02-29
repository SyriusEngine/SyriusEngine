#pragma once

#include "LayerStack.hpp"
#include "../../include/SyriusEngine/SyriusEngine.hpp"

namespace Syrius{

    class SyriusEngineImpl: public SyriusEngine{
    public:
        explicit SyriusEngineImpl(const std::string& configFile);

        ~SyriusEngineImpl() override;

        void run() override;

        void pushLayer(Resource<Layer> layer) override;

        void pushRenderLayer(Resource<RenderLayer> layer) override;

    private:
        LayerStack m_LayerStack;
        EasyIni::Configuration m_Config;
    };

}