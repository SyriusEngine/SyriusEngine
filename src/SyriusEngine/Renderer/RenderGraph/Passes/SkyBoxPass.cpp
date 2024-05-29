#include "SkyBoxPass.hpp"

namespace Syrius{

    SkyBoxPass::SkyBoxPass(ResourceView<Context>& context, Resource<ShaderLibrary>& shaderLibrary, const SkyBoxPassDesc& sbDesc):
    RenderPass(context, shaderLibrary, "SkyBoxPass", PassIDGenerator<SkyBoxPass>::getID()) {

    }

    SkyBoxPass::~SkyBoxPass() {

    }

    void SkyBoxPass::execute(PassMap& passMap) {

    }
}
