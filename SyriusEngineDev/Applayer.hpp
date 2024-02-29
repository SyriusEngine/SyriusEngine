#pragma once

#include "Include.hpp"

namespace SyriusApp{

    class AppLayer: public RenderLayer{
    public:
        explicit AppLayer(Resource<SyriusEngine>& engine);

        ~AppLayer() override;

        void onAttach() override;

        void onDetach() override;

        void onUpdate() override;

        bool onEvent(const Event& event) override;

        void onRendererAttach(ResourceView<Context>& context) override;

        void onRendererDetach(ResourceView<Context>& context) override;

        void onRender(ResourceView<Context>& context) override;

    private:

        void imGuiDrawFrameTimes();

        void imGuiDrawMemoryConsumption();

    private:
        Resource<SyriusEngine>& m_Engine;

        time_t m_DeltaTime;
        time_t m_LastFrameTime;
        std::deque<time_t> m_FrameTimes;


    };


}