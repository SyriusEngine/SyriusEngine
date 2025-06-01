#include "TransformHandle.hpp"
#include "../RenderGraphContainer.hpp"

namespace Syrius::Renderer {

    TransformHandle::TransformHandle(const ResourceView<Context>& ctx, RenderGraphContainer* container):
    IRenderGraphData(ctx, container){
        InstanceData transformData[SR_MAX_INSTANCES];
        ConstantBufferDesc cbDesc;
        cbDesc.name = "TransformData";
        cbDesc.size = sizeof(InstanceData) * SR_MAX_INSTANCES;
        cbDesc.shaderStage = SR_SHADER_VERTEX;
        cbDesc.data = &transformData;
        cbDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        m_TransformBuffer = ctx->createConstantBuffer(cbDesc);
    }



}