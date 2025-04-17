#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include <functional>

namespace Syrius {

    template<typename KEY, typename DATA>
    using CreateFunc = std::function<void(KEY, SP<DATA>)>;

    template<typename KEY, typename DATA>
    using UpdateFunc = std::function<void(KEY, SP<DATA>)>;

    template<typename KEY>
    using DeleteFunc = std::function<void(KEY)>;

}
