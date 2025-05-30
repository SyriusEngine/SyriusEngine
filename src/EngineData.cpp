#include "EngineData.hpp"

namespace Syrius {

    SyriusEngine::EngineData::EngineData(){
        workerPool = createSP<WorkerPool>();
        dispatcherManager = createSP<DispatcherManager>(workerPool);
    }

} // namespace Syrius
