#include <SyriusEngine/EngineConfiguration.hpp>

namespace Syrius {

    EngineConfiguration::EngineConfiguration(const std::vector<std::string_view> &args) {
        for (std::size_t i = 0; i < args.size(); i++) {
            if (args[i] == "-api") {
                const auto value = std::string(args[++i]);
                if (value == "OpenGL") {
                    api = SR_API_OPENGL;
                } else if (value == "D3D11") {
                    api = SR_API_D3D11;
                } else if (value == "Vulkan") {
                    api = SR_API_VULKAN;
                } else {
                    throw std::runtime_error("Invalid API specified");
                }
            } else if (args[i] == "-width") {
                windowWidth = std::stoi(std::string(args[++i]));
            } else if (args[i] == "-height") {
                windowHeight = std::stoi(std::string(args[++i]));
            } else if (args[i] == "-vsync") {
                vsync = true;
            }
        }
    }

}
