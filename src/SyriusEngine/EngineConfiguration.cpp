#include "../../include/SyriusEngine/EngineConfiguration.hpp"

#include <EasyIni/EasyIni.hpp>
#include <utility>

namespace Syrius{

    EngineConfiguration::EngineConfiguration():
    m_ConfigPath(),
    windowWidth(SR_DEFAULT_WIDTH),
    windowHeight(SR_DEFAULT_HEIGHT),
    windowPosX(200),
    windowPosY(200),
    windowTitle("Syrius Engine"),
    graphicsAPI(SR_API_OPENGL),
    enableVSync(true),
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
        enableVSync = config["Context"]["VSync"].getOrDefault<bool>(true);
        clearColor[0] = config["Context"]["ClearR"].getOrDefault<float>(0.1f);
        clearColor[1] = config["Context"]["ClearG"].getOrDefault<float>(0.1f);
        clearColor[2] = config["Context"]["ClearB"].getOrDefault<float>(0.1f);
        clearColor[3] = 1.0f;
        shaderLibraryPath = config["Context"]["ShaderLibrary"].getOrDefault<std::string>("./Resources/Shaders");
        enableSrslShaders = config["Context"]["EnableSrslShaders"].getOrDefault<bool>(true);
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
