#pragma once

#include "../../../include/SyriusEngine/Core/SyriusEngineInclude.hpp"

#include <EasyIni/EasyIni.hpp>

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

    inline UID generateID(){
        return getRandom(1, UINT64_MAX);;
    }

}