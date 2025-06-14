#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Renderer/Material.hpp>
#include <SyriusCore/SyriusCore.hpp>

namespace Syrius {

    using RenderLayerID = UID;

    class SR_ENGINE_API IRenderLayer {
    public:
        IRenderLayer():
        m_ID(generateID()){}

        virtual ~IRenderLayer() = default;

        virtual void onRendererAttach(const ResourceView<Context>& ctx){}

        virtual void onRendererDetach(const ResourceView<Context>& ctx){}

        virtual void onRender(const ResourceView<Context>& ctx) = 0;

        virtual void onResize(u32 width, u32 height, const ResourceView<Context>& ctx){}

        virtual void createMesh(MeshID meshID, const Mesh& mesh, const ResourceView<Context>& ctx){}

        virtual void createInstance(InstanceID instanceID, MeshID meshID, const ResourceView<Context>& ctx){}

        virtual void destroyMesh(MeshID meshID, const ResourceView<Context>& ctx){}

        virtual void destroyInstance(InstanceID instanceID, const ResourceView<Context>& ctx){}

        virtual void setInstanceTransform(InstanceID instanceID, const Transform& transform, const ResourceView<Context>& ctx){}

        virtual void setCamera(CameraID cameraID, const Camera& camera, const ResourceView<Context>& ctx){}

        virtual void createMaterial(MaterialID materialID, const Material& material, const ResourceView<Context>& ctx){}

        virtual void setMeshMaterial(MeshID meshID, MaterialID materialID, const ResourceView<Context>& ctx){}

        virtual void destroyMaterial(MaterialID materialID, const ResourceView<Context>& ctx){}

        virtual void createLight(LightID lightID, const Light& light, const ResourceView<Context>& ctx){}

        virtual void setLight(LightID lightID, const Light& light, const ResourceView<Context>& ctx){}

        virtual void destroyLight(LightID lightID, const ResourceView<Context>& ctx){}

        virtual void setProjection(ProjectionID projectionID, const Projection& projection, const ResourceView<Context>& ctx){}

        RenderLayerID getID() const {
            return m_ID;
        }

    protected:
        const RenderLayerID m_ID = 0;
    };

}
