#include "../../include/SyriusEngine/EngineConfiguration.hpp"
#include "../../include/SyriusEngine/Core/DebugMessageHandler.hpp"

#include <EasyIni/EasyIni.hpp>
#include <utility>

namespace Syrius{

    EngineConfiguration::EngineConfiguration():
    m_ConfigPath("Config.ini"),
    windowWidth(SR_DEFAULT_WIDTH),
    windowHeight(SR_DEFAULT_HEIGHT),
    windowPosX(200),
    windowPosY(200),
    windowTitle("Syrius Engine"),
    graphicsAPI(SR_API_OPENGL),
    enableVSync(false),
    clearColor{0.1f, 0.1f, 0.1f, 1.0f},
    enableSrslShaders(false){

    }

    EngineConfiguration::EngineConfiguration(std::string path):
    m_ConfigPath(std::move(path)){
        EasyIni::Configuration config(m_ConfigPath);
        windowWidth = config["Window"]["Width"].getOrDefault<uint32>(SR_DEFAULT_WIDTH);
        windowHeight = config["Window"]["Height"].getOrDefault<uint32>(SR_DEFAULT_HEIGHT);
        windowPosX = config["Window"]["XPos"].getOrDefault<int32>(200);
        windowPosY = config["Window"]["YPos"].getOrDefault<int32>(200);
        windowTitle = config["Window"]["Title"].getOrDefault<std::string>("Syrius Engine");

        graphicsAPI = static_cast<SR_SUPPORTED_API>(config["Context"]["API"].getOrDefault<int>(SR_API_OPENGL));
        enableVSync = config["Context"]["VSync"].getOrDefault<bool>(false);
        clearColor[0] = config["Context"]["ClearR"].getOrDefault<float>(0.1f);
        clearColor[1] = config["Context"]["ClearG"].getOrDefault<float>(0.1f);
        clearColor[2] = config["Context"]["ClearB"].getOrDefault<float>(0.1f);
        clearColor[3] = 1.0f;
        shaderLibraryPath = config["Context"]["ShaderLibrary"].getOrDefault<std::string>("./Resources/Shaders");
        enableSrslShaders = config["Context"]["EnableSrslShaders"].getOrDefault<bool>(true);
    }

    EngineConfiguration::EngineConfiguration(int argc, char **argv):
    m_ConfigPath("Config.ini"),
    windowWidth(SR_DEFAULT_WIDTH),
    windowHeight(SR_DEFAULT_HEIGHT),
    windowPosX(200),
    windowPosY(200),
    windowTitle("Syrius Engine"),
    graphicsAPI(SR_API_OPENGL),
    enableVSync(false),
    clearColor{0.1f, 0.1f, 0.1f, 1.0f},
    shaderLibraryPath("./Resources/Shaders"),
    enableSrslShaders(false){
        for (uint32 i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "-api") {
                std::string value = argv[++i];
                if (value == "OpenGL") {
                    graphicsAPI = SR_API_OPENGL;
                } else if (value == "D3D11") {
                    graphicsAPI = SR_API_D3D11;
                } else{
                    SR_LOG_WARNING("[EngineConfiguration %p]: Invalid API value: %s, OpenGL will be selected", this, value.c_str());
                }
            }
            else if (arg == "-vsync") {
                enableVSync = true;
            }
            else if (arg == "-config") {

            }
            else{
                SR_LOG_WARNING("[EngineConfiguration %p]: Invalid argument: %s", this, arg.c_str());
            }
        }
    }

    void EngineConfiguration::save() {
        SR_PRECONDITION(!m_ConfigPath.empty(), "[EngineConfiguration %p]: Config path is empty", this);

        save(m_ConfigPath);
    }

    void EngineConfiguration::save(const std::string &path) {
        EasyIni::Configuration config(path);
        config["Window"]["Width"] = windowWidth;
        config["Window"]["Height"] = windowHeight;
        config["Window"]["XPos"] = windowPosX;
        config["Window"]["YPos"] = windowPosY;
        config["Window"]["Title"] = windowTitle;

        config["Context"]["API"] = static_cast<int>(graphicsAPI);
        config["Context"]["VSync"] = enableVSync;
        config["Context"]["ClearR"] = clearColor[0];
        config["Context"]["ClearG"] = clearColor[1];
        config["Context"]["ClearB"] = clearColor[2];
        config["Context"]["ShaderLibrary"] = shaderLibraryPath;
        config["Context"]["EnableSrslShaders"] = enableSrslShaders;

        config.save(path);
    }
}
