#include "MeshPanel.hpp"

namespace Syrius{

    MeshPanel::MeshPanel(Serializer& serializer):
    m_Serializer(serializer),
    m_Meshes(serializer.getMeshes()),
    m_SelectedMesh(-1){

    }

    MeshPanel::~MeshPanel() {
        m_Meshes.clear();
    }

    void MeshPanel::draw() {
        ImGui::Begin("Mesh Panel");
        if (ImGui::BeginListBox("Meshes")){
            for (uint32 i = 0; i < m_Meshes.size(); i++){
                const bool isSelected = (m_SelectedMesh == i);
                std::string descriptor = "Mesh " + std::to_string(i);
                if (ImGui::Selectable(descriptor.c_str(), isSelected)){
                    m_SelectedMesh = i;
                }
                if (isSelected){
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }
        if (m_SelectedMesh != -1){
            drawMeshOptions();
        }

        ImGui::End();
    }

    void MeshPanel::drawMeshOptions() {
        ImGui::BeginChild("Mesh Options");
        float position[3] = {m_Meshes[m_SelectedMesh]->getTranslation().x, m_Meshes[m_SelectedMesh]->getTranslation().y, m_Meshes[m_SelectedMesh]->getTranslation().z};
        if (ImGui::DragFloat3("Position", position, 0.1f)){
            m_Meshes[m_SelectedMesh]->setTranslation({position[0], position[1], position[2]});
        }
        float rotation[3] = {m_Meshes[m_SelectedMesh]->getRotation().x, m_Meshes[m_SelectedMesh]->getRotation().y, m_Meshes[m_SelectedMesh]->getRotation().z};
        if (ImGui::DragFloat3("Rotation", rotation, 0.1f)){
            m_Meshes[m_SelectedMesh]->setRotation({rotation[0], rotation[1], rotation[2]});
        }
        float scale[3] = {m_Meshes[m_SelectedMesh]->getScale().x, m_Meshes[m_SelectedMesh]->getScale().y, m_Meshes[m_SelectedMesh]->getScale().z};
        if (ImGui::DragFloat3("Scale", scale, 0.1f)){
            m_Meshes[m_SelectedMesh]->setScale({scale[0], scale[1], scale[2]});
        }
        if (ImGui::Button("Delete Mesh")){
            m_Meshes.erase(m_Meshes.begin() + m_SelectedMesh);
            m_SelectedMesh = -1;
        }

        ImGui::EndChild();
    }
}
