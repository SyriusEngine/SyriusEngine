#include "ApplicationLayer.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

#define APP_MAX_TIMES 50

namespace Syrius {

    ApplicationLayer::ApplicationLayer(SyriusEngine &engine):
    m_Engine(engine){

    }

    void ApplicationLayer::onAttach() {
        Mesh rectangle;
        rectangle.vertices = {
            {glm::vec3(-0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
            {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
            {glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
            {glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
        };
        rectangle.indices = {
            0, 1, 2,
            0, 2, 3,
    };
        MeshID triangleID = m_Engine.createMesh(rectangle);
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
        if (m_FrameTimes.size() >= APP_MAX_TIMES) {
            m_FrameTimes.pop_front();
        }
        m_FrameTimes.push_back(deltaTime);
    }

    bool ApplicationLayer::onEvent(const Event &event) {
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
        ImGui::Text("%d bytes", allocated); ImGui::NextColumn();
        ImGui::Text("%d KB", allocated / 1024); ImGui::NextColumn();
        ImGui::Text("%d MB", allocated / 1024 / 1024); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Total Freed"); ImGui::NextColumn();
        auto freed = MemoryProfiler::getDeallocatedMemory();
        ImGui::Text("%d bytes", freed); ImGui::NextColumn();
        ImGui::Text("%d KB", freed / 1024); ImGui::NextColumn();
        ImGui::Text("%d MB", freed / 1024 / 1024); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Usage"); ImGui::NextColumn();
        auto usage = MemoryProfiler::getCurrentUsage();
        ImGui::Text("%d bytes", usage); ImGui::NextColumn();
        ImGui::Text("%d KB", usage / 1024); ImGui::NextColumn();
        ImGui::Text("%d MB", usage / 1024 / 1024); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Columns(1);

        ImGui::End();
    }

}
