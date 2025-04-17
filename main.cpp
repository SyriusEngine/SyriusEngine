#include <iostream>
#include <SyriusEngine/SyriusEngine.hpp>
#include "Application/ApplicationLayer.hpp"


int main(int argc, char* argv[]) {
    try{
        // Create Engine
        const std::vector<std::string_view> args(argv + 1, argv + argc);
        const Syrius::EngineConfiguration config(args);
        Syrius::SyriusEngine engine(config);

        // Create Application Layer
        auto appLayer = Syrius::createSP<Syrius::ApplicationLayer>(engine);
        engine.pushLayer(appLayer); // General stuff
        engine.pushRenderLayer(appLayer); // UI stuff

        engine.run();

    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
        return -1;

    }
    return 0;
}
