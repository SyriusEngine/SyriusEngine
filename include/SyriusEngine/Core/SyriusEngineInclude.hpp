#pragma once

#include "DebugMessageHandler.hpp"
#include "Limits.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

#include <memory>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <random>

#include "PlatformDetection.hpp"

namespace Syrius{

    typedef uint64 UID;
    typedef uint64 Index;


    template<typename T>
    using SharedResource = std::shared_ptr<T>;

    template<typename T, typename... Args>
    inline SharedResource<T> createSharedResource(Args&&... args){
        return std::make_shared<T>(args...);
    }

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

