#include "../../include/SyriusEngine/SyriusEngine.hpp"
#include "SyriusEngineImpl.hpp"

namespace Syrius{

    uint64 SyriusEngine::m_InstanceCount = 0;

    SyriusEngine::SyriusEngine(const std::string &configFile):
    m_ConfigFile(configFile){
        if (!m_InstanceCount) {
            setDebugMessageCallback(DebugMessageHandler::receiveSyriusCoreMessage);
        }
        m_InstanceCount++;
    }

    SyriusEngine::~SyriusEngine() {
        m_InstanceCount--;

    }

    Resource<SyriusEngine> createEngine(const std::string& configFile){
        return createResource<SyriusEngineImpl>(configFile);
    }
}
