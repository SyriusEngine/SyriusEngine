#pragma once

#include "IRenderGraphData.hpp"

namespace Syrius::Renderer {

    class RenderGraphContainer {
    public:
        RenderGraphContainer() = default;

        ~RenderGraphContainer() = default;

        template<typename DATA, typename... Args>
        void createData(Args&&... args) {
            static_assert(std::is_base_of_v<IRenderGraphData, DATA>, "DATA is not derived from IRenderGraphData!");

            SP<IRenderGraphData> ptr = createSP<DATA>(std::forward<Args>(args)...);
            m_Data.insert({std::type_index(typeid(DATA)), ptr});
        }

        template<typename DATA>
        SP<DATA> getData() {
            const auto key = std::type_index(typeid(DATA));
            if (m_Data.find(key) == m_Data.end()) {
                SR_LOG_THROW("RenderGraphContainer", "Could not find data!");
            }
            return std::static_pointer_cast<DATA>(m_Data.at(key));
        }

    private:
        std::unordered_map<std::type_index, SP<IRenderGraphData>> m_Data;
    };

}