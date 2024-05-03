#include "Applayer.hpp"

namespace Syrius{


    AppLayer::AppLayer(Resource<SyriusEngine> &engine):
    RenderLayer(),
    m_Engine(engine),
    m_Camera(nullptr),
    m_UseCamera(false),
    m_MeshPanel(nullptr),
    m_Serializer(engine->getRenderCommand()){

    }

    AppLayer::~AppLayer() = default;

    void AppLayer::onAttach() {
        m_Camera = createResource<Camera>(m_Engine->getRenderCommand(), 0.1f, 0.01f);
        m_MeshPanel = createResource<MeshPanel>(m_Serializer);
        m_LightPanel = createResource<LightPanel>(m_Serializer);
    }

    void AppLayer::onDetach() {
       m_Serializer.clear();
    }

    void AppLayer::onUpdate() {
        auto currentTime = getTimeMilli();
        m_DeltaTime = currentTime - m_LastFrameTime;
        if (m_FrameTimes.size() > 50) {
            m_FrameTimes.pop_front();
        }
        m_FrameTimes.push_back(m_DeltaTime);
        m_LastFrameTime = currentTime;

        if (m_OpenFileDialog){
            m_OpenFileDialog = false;
            auto path = m_Engine->getWindow()->openFileDialog("");
            if (!path.empty()){
                m_Serializer.loadScene(path);
            }
        }
    }

    bool AppLayer::onEvent(const Event &event) {
        switch (event.type) {
            case SR_EVENT_WINDOW_CLOSED: m_Engine->shutdown(); break;
            case SR_EVENT_WINDOW_RESIZED: m_Engine->getRenderCommand()->onResize(event.windowWidth, event.windowHeight); break;
            case SR_EVENT_RAW_KEYBOARD_KEY_PRESSED: {
                if (event.keyCode == SR_KEY_ESCAPE){
                    m_Engine->getWindow()->close();
                }
                if (event.keyCode == SR_KEY_F){
                    m_UseCamera = !m_UseCamera;
                    if (m_UseCamera){
                        m_Engine->getWindow()->hideMouse();
                        m_Engine->getWindow()->grabMouse();
                        //m_Engine->getWindow()->enableFullscreen();
                    }
                    else{
                        m_Engine->getWindow()->showMouse();
                        m_Engine->getWindow()->releaseMouse();
                        //m_Engine->getWindow()->disableFullscreen();
                    }
                }
                break;
            }
            default:
                break;
        }

        if (m_UseCamera) {
            m_Camera->update(event, m_DeltaTime);
            return false;
        }
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
        if (m_UseCamera){
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Camera: ON");
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Camera: OFF");
        }
        ImGui::Text("Press F to take/release camera control");

        if (ImGui::Button("Load Scene")){
            m_OpenFileDialog = true;
        }

        imGuiDrawFrameTimes();

        imGuiDrawMemoryConsumption();

        ImGui::End();

        m_MeshPanel->draw();
        m_LightPanel->draw();
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

        auto timers = SR_GET_TIMING_DATA();
        ImGui::Text("Timers");
        ImGui::Columns(2, "Timing Data");
        ImGui::Separator();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("Time"); ImGui::NextColumn();
        ImGui::Separator();
        for (const auto& [name, time]: timers){
            ImGui::Text(name.c_str()); ImGui::NextColumn();
            ImGui::Text("%f ms", time); ImGui::NextColumn();
        }
        ImGui::Separator();
        ImGui::Columns(1);
        ImGui::NewLine();
    }

    void AppLayer::imGuiDrawMemoryConsumption() {
        ImGui::Text("Memory");
        ImGui::Columns(4, "Memory Allocation Tracker");
        ImGui::Separator();
        ImGui::Text("Total Allocated"); ImGui::NextColumn();
        auto allocated = getAllocatedMemory();
        ImGui::Text("%d bytes", allocated); ImGui::NextColumn();
        ImGui::Text("%d KB", allocated / 1024); ImGui::NextColumn();
        ImGui::Text("%d MB", allocated / 1024 / 1024); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Total Freed"); ImGui::NextColumn();
        auto freed = getFreedMemory();
        ImGui::Text("%d bytes", freed); ImGui::NextColumn();
        ImGui::Text("%d KB", freed / 1024); ImGui::NextColumn();
        ImGui::Text("%d MB", freed / 1024 / 1024); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Usage"); ImGui::NextColumn();
        auto usage = getMemoryUsage();
        ImGui::Text("%d bytes", usage); ImGui::NextColumn();
        ImGui::Text("%d KB", usage / 1024); ImGui::NextColumn();
        ImGui::Text("%d MB", usage / 1024 / 1024); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Columns(1);
    }
}
