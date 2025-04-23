#pragma once

#include <SyriusEngine/SyriusEngine.hpp>

namespace Syrius{

    class CameraControl{
    public:
        CameraControl(const SyriusEngine& engine, float sensitivity, float speed);

        ~CameraControl() = default;

        void update(const Event& event, Duration deltaTime);

        void setPosition(const glm::vec3& position);

        void setSensitivity(float sensitivity);

        void setSpeed(float speed);

        void imGuiDrawCameraData() const;

    private:

        void updateCameraData();

        void mouseMoved(float mousePosX, float mousePosY);

        void move(i32 keyPressed, Duration deltaTime);

    private:
        const SyriusEngine& m_Engine;

        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp;

        float m_Yaw;
        float m_Pitch;
        float m_Speed;
        float m_Sensitivity;
    };
}