#include "Applayer.hpp"

namespace SyriusApp{


    AppLayer::AppLayer(Resource<SyriusEngine> &engine):
    RenderLayer(),
    m_Engine(engine){

    }

    AppLayer::~AppLayer() = default;

    void AppLayer::onAttach() {

    }

    void AppLayer::onDetach() {

    }

    void AppLayer::onUpdate() {
        auto currentTime = getTimeMilli();
        m_DeltaTime = currentTime - m_LastFrameTime;
        if (m_FrameTimes.size() > 50) {
            m_FrameTimes.pop_front();
        }
        m_FrameTimes.push_back(m_DeltaTime);
        m_LastFrameTime = currentTime;
    }

    bool AppLayer::onEvent(const Event &event) {
        return true;
    }

    void AppLayer::onRendererAttach(ResourceView<Context> &context) {
        m_Engine->getWindow()->createImGuiContext();
    }

    void AppLayer::onRendererDetach(ResourceView<Context> &context) {
        m_Engine->getWindow()->destroyImGuiContext();
    }

    void AppLayer::onRender(ResourceView<Context> &context) {
        m_Engine->getWindow()->onImGuiBegin();

        ImGui::Begin("Debug");
        imGuiDrawFrameTimes();
        imGuiDrawMemoryConsumption();

        ImGui::End();
        m_Engine->getWindow()->onImGuiEnd();
    }

    void AppLayer::imGuiDrawFrameTimes() {
        ImGui::Text("FPS: %f", 1000.0 / m_DeltaTime);

        float times[50];
        for (uint32 i = 0; i < m_FrameTimes.size(); i++) {
            times[i] = m_FrameTimes[i];
        }
        ImGui::PlotHistogram("FT H", times, m_FrameTimes.size(), 0, nullptr, 0.0f, 40.0f, ImVec2(0, 80));
        ImGui::PlotLines("FT L", times, m_FrameTimes.size(), 0, nullptr, 0.0f, 40.0f, ImVec2(0, 80));
    }

    void AppLayer::imGuiDrawMemoryConsumption() {
        ImGui::Text("Memory");
        ImGui::Columns(2, "Memory Allocation Tracker");
        ImGui::Separator();
        ImGui::Text("Total Allocated"); ImGui::NextColumn();
        ImGui::Text("%d bytes", getAllocatedMemory()); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Total Freed"); ImGui::NextColumn();
        ImGui::Text("%d bytes", getFreedMemory()); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Usage"); ImGui::NextColumn();
        ImGui::Text("%d bytes", getMemoryUsage()); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Columns(1);
    }
}
