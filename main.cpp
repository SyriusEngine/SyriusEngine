#include <iostream>
#include "include/SyriusEngine/SyriusEngine.hpp"

int main(int argc, char** argv) {
    try{
        if (argc < 2){
            std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
            return 1;
        }
        auto engine =  Syrius::createEngine(argv[1]);
        engine->run();

    }catch (std::exception& e){
        std::cerr << e.what() << std::endl;
        return -1;

    } catch (...){
        std::cerr << "Unknown Error" << std::endl;
        return -1;
    }

    printf("\nTotal Allocated Memory: %zu bytes\n",  Syrius::getAllocatedMemory());
    printf("Total Deallocated Memory: %zu bytes\n",  Syrius::getFreedMemory());
    printf("Lost Memory: %zu bytes\n",  Syrius::getMemoryUsage());
    return 0;
}
