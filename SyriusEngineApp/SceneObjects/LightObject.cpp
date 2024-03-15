#include "LightObject.hpp"

namespace Syrius{

    LightObject::LightObject(const ResourceView<RenderCommand> &renderCommand, glm::vec3 position, glm::vec3 color):
    m_RenderCommand(renderCommand),
    m_Position(position),
    m_Color(color),
    m_LightID(0){
        Light light;
        light.position = m_Position;
        light.color = m_Color;
        m_LightID = m_RenderCommand->createLight(light);
    }

    LightObject::~LightObject() {
        m_RenderCommand->removeLight(m_LightID);
    }

    void LightObject::setPosition(const glm::vec3 &position) {
        m_Position = position;
        Light light;
        light.position = m_Position;
        light.color = m_Color;
        m_RenderCommand->updateLight(m_LightID, light);
    }

    void LightObject::setColor(const glm::vec3 &color) {
        m_Color = color;
        Light light;
        light.position = m_Position;
        light.color = m_Color;
        m_RenderCommand->updateLight(m_LightID, light);
    }

    const glm::vec3 &LightObject::getPosition() const {
        return m_Position;
    }

    const glm::vec3 &LightObject::getColor() const {
        return m_Color;
    }
}
