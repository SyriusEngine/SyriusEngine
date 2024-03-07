#pragma once

#include "Include.hpp"
#include "Camera.hpp"
#include "PanelsUI/MeshPanel.hpp"

namespace Syrius{

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

        static void imGuiDrawMemoryConsumption();

    private:
        Resource<SyriusEngine>& m_Engine;
        Resource<Camera> m_Camera;

        Resource<MeshPanel> m_MeshPanel;

        std::vector<MeshID> m_Meshes;

        bool m_UseCamera;

        time_t m_DeltaTime;
        time_t m_LastFrameTime;
        std::deque<time_t> m_FrameTimes;

        Worker m_Dispatcher;


    };


}