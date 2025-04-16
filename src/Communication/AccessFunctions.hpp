#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include <functional>

namespace Syrius {

    template<typename T>
    using CreateFunc = std::function<void(UID, SP<T>)>;

    template<typename T>
    using UpdateFunc = std::function<void(UID, SP<T>)>;

    template<typename T>
    using DeleteFunc = std::function<void(UID)>;

}
