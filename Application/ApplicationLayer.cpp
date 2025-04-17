#include "ApplicationLayer.hpp"

namespace Syrius {

    ApplicationLayer::ApplicationLayer(SyriusEngine &engine):
    m_Engine(engine){

    }

    void ApplicationLayer::onAttach() {

    }

    void ApplicationLayer::onDetach() {

    }

    void ApplicationLayer::onRendererAttach(const ResourceView<Context> &ctx) {
        m_Engine.getWindow()->createImGuiContext();
    }

    void ApplicationLayer::onRendererDetach(const ResourceView<Context> &ctx) {
        m_Engine.getWindow()->destroyImGuiContext();
    }

    void ApplicationLayer::onUpdate() {

    }

    bool ApplicationLayer::onEvent(const Event &event) {
        return true;
    }


    void ApplicationLayer::onRender(const ResourceView<Context> &ctx) {
        m_Engine.getWindow()->onImGuiBegin();

        ImGuiDebugWindow(ctx);

        m_Engine.getWindow()->onImGuiEnd();
    }

    void ApplicationLayer::ImGuiDebugWindow(const ResourceView<Context> &ctx) {
        ImGui::Begin("Debug Window");

        static float backgroundColor[3] = {0.0f, 0.0f, 0.0f};
        if (ImGui::ColorEdit3("Background Color", backgroundColor)) {
            ctx->setClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
        }

        ImGui::End();
    }

}
