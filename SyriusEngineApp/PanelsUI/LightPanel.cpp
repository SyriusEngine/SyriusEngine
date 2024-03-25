#include "LightPanel.hpp"

namespace Syrius{

    LightPanel::LightPanel(Serializer& serializer):
    m_Serializer(serializer),
    m_SelectedLight(-1),
    m_Lights(serializer.getLights()){

    }

    LightPanel::~LightPanel() {

    }

    void LightPanel::draw() {
        ImGui::Begin("Light Panel");

        if (ImGui::BeginListBox("Lights")) {
            for (uint32 i = 0; i < m_Lights.size(); i++) {
                const bool isSelected = (m_SelectedLight == i);
                std::string description = "Light " + std::to_string(i);
                if (ImGui::Selectable(description.c_str(), isSelected)) {
                    m_SelectedLight = i;
                }
                if (isSelected) {
                   ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }
        if (m_SelectedLight != -1) {
            drawLightOptions();
        }

        ImGui::End();
    }

    void LightPanel::drawLightOptions() {
        ImGui::BeginChild("Light Options");
        float position[3] = {m_Lights[m_SelectedLight]->getPosition().x,
                             m_Lights[m_SelectedLight]->getPosition().y,
                             m_Lights[m_SelectedLight]->getPosition().z};
        if (ImGui::DragFloat3("Position", position, 0.1f)){
            m_Lights[m_SelectedLight]->setPosition(glm::vec3(position[0], position[1], position[2]));
        }
        float color[3] = {m_Lights[m_SelectedLight]->getColor().x,
                            m_Lights[m_SelectedLight]->getColor().y,
                            m_Lights[m_SelectedLight]->getColor().z};
        if (ImGui::ColorEdit3("Color", color)){
            m_Lights[m_SelectedLight]->setColor(glm::vec3(color[0], color[1], color[2]));
        }

        if (ImGui::Button("Delete Light")){
            m_Lights.erase(m_Lights.begin() + m_SelectedLight);
            m_SelectedLight = -1;
        }
        ImGui::EndChild();
    }
}
