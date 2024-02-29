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

}

