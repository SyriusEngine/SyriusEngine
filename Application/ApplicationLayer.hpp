#pragma once

#include <SyriusEngine/SyriusEngine.hpp>

namespace Syrius {

    class ApplicationLayer: public ILayer, public IRenderLayer { // Inherit from render layer to draw UI
    public:
        explicit ApplicationLayer(SyriusEngine& engine);

        ~ApplicationLayer() override = default;

        void onAttach() override;

        void onDetach() override;

        void onRendererAttach(const ResourceView<Context> &ctx) override;

        void onRendererDetach(const ResourceView<Context> &ctx) override;

        void onUpdate() override;

        bool onEvent(const Event& event) override;

        void onRender(const ResourceView<Context> &ctx) override;

    private:

        void ImGuiDebugWindow(const ResourceView<Context> &ctx);

    private:
        SyriusEngine &m_Engine;
    };

}