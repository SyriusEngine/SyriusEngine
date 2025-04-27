#include "ApplicationLayer.hpp"

#define APP_MAX_TIMES 50

namespace Syrius {

    ApplicationLayer::ApplicationLayer(SyriusEngine &engine):
    m_Engine(engine),
    m_CameraControl(engine, 0.1f, 0.01f){

    }

    void ApplicationLayer::onAttach() {
        // The max instance count SR_MAX_INSTANCES is 512, draw 512 cubes around 0.0f

        // First create the mesh
        const Mesh cube = createCube();
        MeshID cubeID = m_Engine.createMesh(cube);

        // Arrange the instances in a 16 x 16 x 16 grid
        constexpr i32 border = 4;
        for (int x = -border; x < border; x++) {
            for (int y = -border; y < border; y++) {
                for (int z = -border; z < border; z++) {
                    Transform transform;
                    transform.model = glm::mat4(1.0f);
                    transform.model = glm::translate(glm::vec3(x * 2.0f, y * 2.0f, z * 2.0f));

                    const InstanceID instance = m_Engine.createInstance(cubeID);
                    m_Engine.setInstanceTransform(instance, transform);
                }
            }
        }

        // Load the Chipped Paint Metal material
        auto chippedPaintMetalTextures = createSP<Material>(
            "./Resources/Materials/chipped-paint-metal/chipped-paint-metal_basecolor.png",
            "./Resources/Materials/chipped-paint-metal/chipped-paint-metal_normal.png",
            "./Resources/Materials/chipped-paint-metal/chipped-paint-metal_metallic.png",
            "./Resources/Materials/chipped-paint-metal/chipped-paint-metal_roughness.png",
            "./Resources/Materials/chipped-paint-metal/chipped-paint-metal_ao.png"
        );
        MaterialID chippedPaintMetal = m_Engine.createMaterial(chippedPaintMetalTextures);
        m_Engine.meshSetMaterial(cubeID, chippedPaintMetal);
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
