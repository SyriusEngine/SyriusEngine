#include "../../include/SyriusEngine/SyriusEngine.hpp"
#include "SyriusEngineImpl.hpp"

namespace Syrius{

    uint64 SyriusEngine::m_InstanceCount = 0;

    SyriusEngine::SyriusEngine(const EngineConfiguration& config):
    m_Config(config){
        if (!m_InstanceCount) {
            setDebugMessageCallback(DebugMessageHandler::receiveSyriusCoreMessage);
        }
        m_InstanceCount++;
    }

    SyriusEngine::~SyriusEngine() {
        m_InstanceCount--;

    }

    Resource<SyriusEngine> createEngine(const EngineConfiguration& config){
        return createResource<SyriusEngineImpl>(config);
    }

    void setDebugMessageHandler(HandleDebugMessageFunc cb){
        DebugMessageHandler::setDebugMessageHandler(cb);
    }
}
