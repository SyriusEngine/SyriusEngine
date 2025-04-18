#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>

namespace Syrius {

    class SR_ENGINE_API EngineConfiguration {
    public:
        explicit EngineConfiguration(const std::vector<std::string_view>& args);

        ~EngineConfiguration() = default;

    public:
        SR_SUPPORTED_API api = SR_API_OPENGL; // OpenGL by default
        u32 windowWidth = SR_DEFAULT_WIDTH;
        u32 windowHeight = SR_DEFAULT_HEIGHT;
        bool vsync = true;
        fs::path shaderDirectory = fs::current_path() / "Resources" / "Shaders";

    };
}