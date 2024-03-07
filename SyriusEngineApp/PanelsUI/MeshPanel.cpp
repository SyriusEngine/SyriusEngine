#include "MeshPanel.hpp"

namespace Syrius{

    MeshPanel::MeshPanel(const ResourceView<RenderCommand> &renderCommand, Worker& dispatcher):
    m_RenderCommand(renderCommand),
    m_Dispatcher(dispatcher),
    m_SelectedMesh(-1){

    }

    MeshPanel::~MeshPanel() {

    }

    void MeshPanel::draw() {
        ImGui::Begin("Mesh Panel");

        if (ImGui::Button("Create Random Mesh")){
            m_Dispatcher.addTask([this]{
                createRandomMesh();
            });
        }
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

    void MeshPanel::createRandomMesh() {
        auto random = getRandom(0, 4);
        MeshDesc mesh;
        switch (random) {
            case 0: createCube(mesh); break;
            case 1: createSphere(mesh); break;
            case 2: createCone(mesh); break;
            case 3: createPyramid(mesh); break;
            default: createCube(mesh); break;
        }
        auto newMesh = createResource<Mesh>(mesh, m_RenderCommand);
        m_Meshes.push_back(std::move(newMesh));
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
        ImGui::EndChild();
    }
}
