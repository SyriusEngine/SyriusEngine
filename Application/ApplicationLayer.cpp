#include "ApplicationLayer.hpp"

#define APP_MAX_TIMES 50

namespace Syrius {

    ApplicationLayer::ApplicationLayer(SyriusEngine &engine):
    m_Engine(engine),
    m_CameraControl(engine, 0.1f, 0.01f){

    }

    void ApplicationLayer::onAttach() {
        Mesh cube;
       cube.vertices = {
                /// front
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// left
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// bottom
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// back
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// right
                {glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                /// top
                {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        };
        cube.indices = {
                0, 3, 2,
                0, 2, 1,

                4, 7, 6,
                4, 6, 5,

                8, 11, 10,
                8, 10, 9,

                12, 15, 14,
                12, 14, 13,

                16, 19, 18,
                16, 18, 17,

                20, 23, 22,
                20, 22, 21
        };
        MeshID triangleID = m_Engine.createMesh(cube);
        InstanceID leftUnder = m_Engine.createInstance(triangleID);
        InstanceID rightUnder = m_Engine.createInstance(triangleID);
        InstanceID leftUpper = m_Engine.createInstance(triangleID);
        InstanceID rightUpper = m_Engine.createInstance(triangleID);

        glm::vec3 transformations[4] = {
            {-0.5f, -0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {-0.5f, 0.5f, 0.0f},
            {0.5f, 0.5f, 0.0f}
        };
        std::vector<InstanceID> instances = {leftUnder, rightUnder, leftUpper, rightUpper};
        for (u32 i = 0; i < instances.size(); i++) {
            Transform transform;
            transform.model = glm::translate(glm::vec3(transformations[i]));
            m_Engine.setInstanceTransform(instances[i], transform);
        }
    }

    void ApplicationLayer::onDetach() {

    }

    void ApplicationLayer::onRendererAttach(const ResourceView<Context> &ctx) {
        m_Engine.getWindow()->createImGuiContext();
    }

    void ApplicationLayer::onRendererDetach(const ResourceView<Context> &ctx) {
        m_Engine.getWindow()->destroyImGuiContext();
    }

    void ApplicationLayer::onUpdate(const Duration deltaTime) {
        m_LastFrameTime = deltaTime;
        if (m_FrameTimes.size() >= APP_MAX_TIMES) {
            m_FrameTimes.pop_front();
        }
        m_FrameTimes.push_back(deltaTime);
    }

    bool ApplicationLayer::onEvent(const Event &event) {
        m_CameraControl.update(event, m_LastFrameTime);
        return true;
    }


    void ApplicationLayer::onRender(const ResourceView<Context> &ctx) {
        m_Engine.getWindow()->onImGuiBegin();

        imGuiDebugWindow(ctx);

        m_Engine.getWindow()->onImGuiEnd();
    }

    void ApplicationLayer::imGuiDebugWindow(const ResourceView<Context> &ctx) {
        ImGui::Begin("Debug Window");

        static float backgroundColor[3] = {0.0f, 0.0f, 0.0f};
        if (ImGui::ColorEdit3("Background Color", backgroundColor)) {
            ctx->setClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
        }

        float times[APP_MAX_TIMES];
        for (u32 i = 0; i < m_FrameTimes.size(); i++) {
            times[i] = m_FrameTimes[i].count();
        }
        ImGui::PlotHistogram("FT H", times, m_FrameTimes.size(), 0, nullptr, 0.0f, 40.0f, ImVec2(0, 80));
        ImGui::PlotLines("FT L", times, m_FrameTimes.size(), 0, nullptr, 0.0f, 40.0f, ImVec2(0, 80));

        ImGui::Text("Memory");
        ImGui::Columns(4, "Memory Allocation Tracker");
        ImGui::Separator();
        ImGui::Text("Total Allocated"); ImGui::NextColumn();
        auto allocated = MemoryProfiler::getAllocatedMemory();
        ImGui::Text("%lu bytes", allocated); ImGui::NextColumn();
        ImGui::Text("%lu KB", allocated / 1024); ImGui::NextColumn();
        ImGui::Text("%lu MB", allocated / 1024 / 1024); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Total Freed"); ImGui::NextColumn();
        auto freed = MemoryProfiler::getDeallocatedMemory();
        ImGui::Text("%lu bytes", freed); ImGui::NextColumn();
        ImGui::Text("%lu KB", freed / 1024); ImGui::NextColumn();
        ImGui::Text("%lu MB", freed / 1024 / 1024); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Usage"); ImGui::NextColumn();
        auto usage = MemoryProfiler::getCurrentUsage();
        ImGui::Text("%lu bytes", usage); ImGui::NextColumn();
        ImGui::Text("%lu KB", usage / 1024); ImGui::NextColumn();
        ImGui::Text("%lu MB", usage / 1024 / 1024); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Columns(1);

        ImGui::End();
    }

}
