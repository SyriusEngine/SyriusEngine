#pragma once

#include "Include.hpp"

namespace Syrius{

    class Mesh{
    public:
        Mesh(const MeshDesc& desc, const ResourceView<RenderCommand>& rdCmd);

        ~Mesh();

        void setTranslation(const glm::vec3& translation);

        void setRotation(const glm::vec3& rotation);

        void setScale(const glm::vec3& scale);

        [[nodiscard]] const glm::vec3& getTranslation() const;

        [[nodiscard]] const glm::vec3& getRotation() const;

        [[nodiscard]] const glm::vec3& getScale() const;

        void setMaterial(MaterialID materialID);

    private:

        void calculateTransformation();

    private:
        const ResourceView<RenderCommand>& m_RenderCommand;

        glm::vec3 m_Translation;
        glm::vec3 m_Rotation;
        glm::vec3 m_Scale;

        MeshID m_MeshID;

    };

}