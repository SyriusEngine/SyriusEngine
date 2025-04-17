#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include <SyriusCore/SyriusCore.hpp>

namespace Syrius {

    using RenderLayerID = UID;

    class IRenderLayer {
    public:
        IRenderLayer():
        m_ID(generateID()){}

        virtual ~IRenderLayer();

        virtual void onRender(const ResourceView<Context>& ctx) = 0;

        virtual void onResize(u32 width, u32 height){}

        RenderLayerID getID() const {
            return m_ID;
        }

    protected:
        const RenderLayerID m_ID = 0;
    };

}
