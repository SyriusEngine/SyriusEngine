#pragma once

#include "Include.hpp"

namespace Syrius{

    class Camera{
    public:
        Camera(const ResourceView<RenderCommand>& renderer, float sensitivity, float speed);

        ~Camera() = default;

        void update(const Event& event, time_t deltaTime);

        void setPosition(const glm::vec3& position);

        void setSensitivity(float sensitivity);

        void setSpeed(float speed);

        void imGuiDrawCameraData() const;

    private:

        void updateCameraData();

        void mouseMoved(float mousePosX, float mousePosY);

        void move(int32 keyPressed, time_t deltaTime);

    private:
        const ResourceView<RenderCommand>& m_Renderer;

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