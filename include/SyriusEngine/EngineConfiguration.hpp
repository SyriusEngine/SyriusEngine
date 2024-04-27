#pragma once

#include "Core/SyriusEngineInclude.hpp"

namespace Syrius{

    class SR_API EngineConfiguration{
    public:
        /**
         * @brief Default constructor, will initialise all engine settings with default values
         */
        EngineConfiguration();

        /**
         * @brief Constructor that will load the engine settings from the specified file. A Ini::Configuration
         *        object will be created from the file and the settings will be loaded from it.
         * @param path The path to the file containing the engine settings
         */
        explicit EngineConfiguration(std::string  path);

        /**
         * @brief Constructor that will load the engine settings from command line arguments. The command line
         *        arguments will be parsed and the settings will be loaded from them.
         * @param argc The number of arguments
         * @param argv The arguments
         */
        EngineConfiguration(int argc, char** argv);

        void save();

        void save(const std::string& path);

    public:
        const std::string m_ConfigPath;

        // Window settings
        uint32 windowWidth;
        uint32 windowHeight;
        int32 windowPosX;
        int32 windowPosY;
        std::string windowTitle;

        // Context settings
        SR_SUPPORTED_API graphicsAPI;
        bool enableVSync;
        float clearColor[4];
        std::string shaderLibraryPath;
        bool enableSrslShaders;



    };
}