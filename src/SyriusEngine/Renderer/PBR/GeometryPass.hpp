#pragma once

#include "../RenderPass.hpp"
#include "MeshHandle.hpp"

#include <SRSTL/KeyVector.hpp>


namespace Syrius{

    struct GeometryPassDesc{
        uint32 modelDataBufferSlot;
        std::string modelDataBufferName;
    };

    class GeometryPass : public RenderPass{
    public:
        GeometryPass(ResourceView<Context>& context,  Resource<ShaderLibrary>& shaderLibrary, const GeometryPassDesc& gpDesc);

        ~GeometryPass() override;

        void execute(PassMap& passMap) override;

        MeshID createMesh(const MeshDesc& mesh);

        void transformMesh(MeshID meshID, const glm::mat4& transform);

        void removeMesh(MeshID meshID);

    private:
        Srstl::KeyVector<MeshID, MeshHandle> m_Meshes;

        ResourceView<ConstantBuffer> m_ModelData;
        uint32 m_Slot;

    };

}