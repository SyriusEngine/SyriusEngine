#pragma once

#include "../RenderPass.hpp"

namespace Syrius{

    struct SkyBoxPassDesc{
        Resource<Image> radianceMap;
    };

    class SkyBoxPass: public RenderPass{
    public:
        SkyBoxPass(ResourceView<Context>& context, Resource<ShaderLibrary>& shaderLibrary, const SkyBoxPassDesc& sbDesc);

        ~SkyBoxPass() override;

        void execute(PassMap& passMap) override;

    private:

    };
}