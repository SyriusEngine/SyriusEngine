#pragma once

#include "../Include.hpp"

namespace Syrius{

    class LightObject{
    public:
        LightObject(const ResourceView<RenderCommand>& renderCommand, glm::vec3 position, glm::vec3 color);

        ~LightObject();

        void setPosition(const glm::vec3& position);

        void setColor(const glm::vec3& color);

        [[nodiscard]] const glm::vec3& getPosition() const;

        [[nodiscard]] const glm::vec3& getColor() const;

    private:
        const ResourceView<RenderCommand>& m_RenderCommand;

        glm::vec3 m_Position;
        glm::vec3 m_Color;

        LightID m_LightID;
    };
}