#include <iostream>
#include <SyriusEngine/SyriusEngine.hpp>


int main(int argc, char* argv[]) {
    try{
        const std::vector<std::string_view> args(argv + 1, argv + argc);
        const Syrius::EngineConfiguration config(args);
        Syrius::SyriusEngine engine(config);
        engine.run();

    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
        return -1;

    }
    return 0;
}
