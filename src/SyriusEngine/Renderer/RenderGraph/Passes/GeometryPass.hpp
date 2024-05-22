#pragma once

#include "../RenderPass.hpp"
#include "MeshHandle.hpp"
#include "MaterialHandle.hpp"

#include "SRSTL/KeyVector.hpp"


namespace Syrius{

    struct GeometryPassDesc{
        uint32 modelDataBufferSlot;
        uint32 materialStartingSlot = 0;
        std::string modelDataBufferName;
    };

    class GeometryPass : public RenderPass{
    public:
        GeometryPass(ResourceView<Context>& context, Resource<ShaderLibrary>& shaderLibrary, const GeometryPassDesc& gpDesc);

        ~GeometryPass() override;

        void execute(PassMap& passMap) override;

        MeshID createMesh(const MeshDesc& mesh);

        MeshID createMesh(MeshID meshID);

        void transformMesh(MeshID meshID, const glm::mat4& transform);

        void removeMesh(MeshID meshID);

        MaterialID createMaterial(const MaterialDesc& material);

        void setMeshMaterial(MeshID meshID, MaterialID materialID);

        void removeMaterial(MaterialID materialID);

    private:

        void createDefaultMaterial();

    private:
        Srstl::KeyVector<MeshDataID, MeshHandle> m_Meshes;
        Srstl::KeyVector<MaterialID, MaterialHandle> m_Materials;

        std::unordered_map<MeshID, MeshDataID> m_MeshInstances;

        ResourceView<ConstantBuffer> m_ModelData;
        uint32 m_Slot;
        uint32 m_MaterialStartSlot;

    };

}