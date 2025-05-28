#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Utils/EngineLimits.hpp>
#include <SyriusUtils/Srstl/KeyVector.hpp>

#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    class TransformHandle {
    public:
        explicit TransformHandle(const ResourceView<Context>& ctx);

        ~TransformHandle() = default;

        inline void bind(const u32 slot) const {
            m_TransformBuffer->bindShaderResource(slot);
        }

        inline void setData(const Srstl::KeyVector<InstanceID, InstanceData>& data) const {
            SR_PRECONDITION(data.getSize() <= sizeof(InstanceData) * SR_MAX_INSTANCES, "Transform data size is not equal to transform buffer size");

            m_TransformBuffer->setData(data.getData().data(), data.getSize() * sizeof(InstanceData));
        }

    private:
        ResourceView<ConstantBuffer> m_TransformBuffer;

    };

}