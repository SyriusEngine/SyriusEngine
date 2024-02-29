#pragma once

#include "../../../include/SyriusEngine/Core/SyriusEngineInclude.hpp"

#include <EasyIni/EasyIni.hpp>

#include <chrono>
#include <random>
#include <filesystem>
#include <mutex>
#include <thread>
#include <iostream>
#include <algorithm>
#include <functional>

#define SR_DEFAULT_WINDOW_WIDTH 1280
#define SR_DEFAULT_WINDOW_HEIGHT 720

namespace Syrius{

    inline time_t getTimeMilli(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
    }

    inline uint64 getRandom(uint64 min, uint64 max){
        std::random_device rd;
        Time seeder = getTimeMilli();
        std::mt19937_64::result_type seed = rd() ^ seeder;
        std::mt19937_64 gen(seed);
        std::uniform_int_distribution<uint64> dist(min, max);
        return dist(gen);
    }

    inline UID generateID(){
        return getRandom(1, UINT64_MAX);;
    }

}