#include "CameraControl.hpp"

namespace Syrius{

    CameraControl::CameraControl(const SyriusEngine& engine, float sensitivity, float speed):
    m_Engine(engine),
    m_Position(glm::vec3(0.0f, 0.0f, 3.0f)),
    m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_Right(glm::vec3(1.0f, 0.0f, 0.0f)),
    m_WorldUp({0.0f, 1.0f, 0.0f}),
    m_Yaw(-90.0f),
    m_Pitch(0.0f),
    m_Speed(speed),
    m_Sensitivity(sensitivity){
        updateCameraData();
    }

    void CameraControl::update(const Event &event, Duration deltaTime) {
        switch (event.type) {
            case SR_EVENT_RAW_MOUSE_MOVED: {
                auto xPos = static_cast<float>(event.mousePosX);
                auto yPos = static_cast<float>(event.mousePosY);
                mouseMoved(xPos, yPos);
                break;
            }
            case SR_EVENT_KEYBOARD_KEY_PRESSED: {
                auto key = event.keyCode;
                move(key, deltaTime);
                break;
            }
            default:
                break;
        }
    }

    void CameraControl::setPosition(const glm::vec3 &position) {
        m_Position = position;
    }

    void CameraControl::setSensitivity(float sensitivity) {
        m_Sensitivity = sensitivity;
    }

    void CameraControl::setSpeed(float speed) {
        m_Speed = speed;
    }

    void CameraControl::updateCameraData() {
        glm::vec3 front;
        front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
        front.y = glm::sin(glm::radians(m_Pitch));
        front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

        m_Front = glm::normalize(front);
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        Camera camera;
        camera.view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
        camera.position = glm::vec4(m_Position, 1.0f);

        m_Engine.setCamera(SR_DEFAULT_CAMERA, camera);
    }

    void CameraControl::mouseMoved(float mousePosX, float mousePosY) {
        mousePosX *= m_Sensitivity;
        mousePosY *= m_Sensitivity;

        m_Yaw += mousePosX;
        m_Pitch -= mousePosY;

        if (m_Pitch > 89.0f) {
            m_Pitch = 89.0f;
        }
        if (m_Pitch < -89.0f) {
            m_Pitch = -89.0f;
        }

        updateCameraData();
    }

    void CameraControl::move(const i32 keyPressed, const Duration deltaTime) {
        float velocity = static_cast<float>(deltaTime.count()) * m_Speed;
        switch (keyPressed) {
            case SR_KEY_W:  m_Position += m_Front * velocity; break;
            case SR_KEY_S:  m_Position -= m_Front * velocity; break;
            case SR_KEY_A:  m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * velocity; break;
            case SR_KEY_D:  m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * velocity; break;
            case SR_KEY_SPACE:  m_Position += m_Up * velocity; break;
            case SR_KEY_LEFT_CONTROL:  m_Position -= m_Up * velocity; break;
            default: break;
        }
        updateCameraData();
    }

    void CameraControl::imGuiDrawCameraData() const {
        ImGui::Text("Camera Data");
        ImGui::Columns(4, "Camera Properties");
        ImGui::Separator();
        ImGui::Text("Property"); ImGui::NextColumn();
        ImGui::Text("X"); ImGui::NextColumn();
        ImGui::Text("Y"); ImGui::NextColumn();
        ImGui::Text("Z"); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Position"); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Position.x); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Position.y); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Position.z); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Front"); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Front.x); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Front.y); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Front.z); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Up"); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Up.x); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Up.y); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Up.z); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("Right"); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Right.x); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Right.y); ImGui::NextColumn();
        ImGui::Text("%.2f", m_Right.z); ImGui::NextColumn();
        ImGui::Separator();

        ImGui::Columns(1);

    }
}
