#pragma once

#include "../Pass.hpp"

namespace Syrius{

    class GBufferPass : public Pass{
    public:
        explicit GBufferPass(ResourceView<Context>& context);

        ~GBufferPass() override;

        void execute(PassMap& passMap) override;

        void beginRenderPass();

        void endRenderPass();

        void bindShaderResources();

    private:
        ResourceView<FrameBuffer> m_FrameBuffer;

    };

}