#include "Mesh.hpp"

namespace Syrius{

    Mesh::Mesh(const MeshDesc &desc, const ResourceView<RenderCommand> &rdCmd):
    m_MeshID(0),
    m_RenderCommand(rdCmd),
    m_Translation(0.0f),
    m_Rotation(0.0f),
    m_Scale(1.0f){
        m_MeshID = m_RenderCommand->createMesh(desc);
    }

    Mesh::~Mesh() {

    }

    void Mesh::setTranslation(const glm::vec3 &translation) {
        m_Translation = translation;
        calculateTransformation();
    }

    void Mesh::setRotation(const glm::vec3 &rotation) {
        m_Rotation = rotation;
        calculateTransformation();
    }

    void Mesh::setScale(const glm::vec3 &scale) {
        m_Scale = scale;
        calculateTransformation();
    }

    const glm::vec3 &Mesh::getTranslation() const {
        return m_Translation;
    }

    const glm::vec3 &Mesh::getRotation() const {
        return m_Rotation;
    }

    const glm::vec3 &Mesh::getScale() const {
        return m_Scale;
    }

    void Mesh::calculateTransformation() {
        glm::mat4 translate = glm::translate(m_Translation);
        glm::mat4 rotate = glm::toMat4(glm::quat(m_Rotation));
        glm::mat4 scale = glm::scale(m_Scale);

        glm::mat4 transformation = translate * rotate * scale;
        m_RenderCommand->transformMesh(m_MeshID, transformation);

    }
}
