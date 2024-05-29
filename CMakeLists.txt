cmake_minimum_required(VERSION 3.16)
project(SyriusEngine)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(SR_EXPORT_DLL)
add_compile_definitions(SR_DEBUG)

add_subdirectory(./Dependencies/SRSLCompiler/)
add_subdirectory(./Dependencies/SyriusCore/)
add_subdirectory(./Dependencies/googletest/)

set(SR_DEPENDENCIES_DLL
    ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/SyriusCore/libSyriusCore.dll
    ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/SRSLCompiler/libSRSLCompiler.dll
    ${CMAKE_CURRENT_BINARY_DIR}/../Dependencies/assimp/bin/libassimp-5.dll
    )

add_custom_target(CopyDLL ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SR_DEPENDENCIES_DLL} ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${VISU3D_DEPENDECIES_DLL}
    COMMENT "Copying DLLs to build tree")
add_dependencies(CopyDLL SyriusCore SRSLCompiler)

include_directories(./Dependencies/SyriusCore/include/)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/Dependencies/SyriusCore/)

include_directories(./Dependencies/SRSLCompiler/include/)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/Dependencies/SRSLCompiler/)

include_directories(./Dependencies/glm/)

include_directories(./Dependencies/EasyIni/include/)

include_directories(./Dependencies/SRSTL/include/)

set(SR_ENGINE_CORE_SRC
    src/SyriusEngine/Core/DebugMessageHandler.cpp
    src/SyriusEngine/Core/InternalInclude.cpp
    src/SyriusEngine/Core/Profiler.cpp)

set(SR_ENGINE_RENDERER_SRC
    src/SyriusEngine/Renderer/Primitives.cpp
    src/SyriusEngine/Renderer/Renderer.cpp

    src/SyriusEngine/Renderer/RenderGraph/RenderPass.cpp
    src/SyriusEngine/Renderer/RenderGraph/RenderGraph.cpp
    src/SyriusEngine/Renderer/RenderGraph/Pass.cpp
    src/SyriusEngine/Renderer/RenderGraph/PassMap.cpp
    src/SyriusEngine/Renderer/ShaderLibrary.cpp

    src/SyriusEngine/Renderer/RenderGraph/RenderGraphLayer.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/ProjectionPass.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/CameraDataPass.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/LFWRSamplerPass.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/MeshHandle.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/GeometryPass.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/MaterialHandle.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/GBufferPass.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/LightDataPass.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/LightPass.cpp
    src/SyriusEngine/Renderer/RenderGraph/Passes/SkyBoxPass.cpp src/SyriusEngine/Renderer/RenderGraph/Passes/SkyBoxPass.hpp)

set(SR_ENGINE_ECS_SRC
    src/SyriusEngine/EntityComponentSystem/Component.cpp
    src/SyriusEngine/EntityComponentSystem/EntityComponentSystem.cpp
    )


set(SR_ENGINE_UTILS_SRC
    src/SyriusEngine/Utils/Worker.cpp)

set(SR_ENGINE_SRC
    ${SR_ENGINE_CORE_SRC}
    ${SR_ENGINE_RENDERER_SRC}
    ${SR_ENGINE_UTILS_SRC}
    ${SR_ENGINE_ECS_SRC}
    src/SyriusEngine/SyriusEngine.cpp
    src/SyriusEngine/LayerStack.cpp
    src/SyriusEngine/BuiltIns.cpp
    src/SyriusEngine/SyriusEngineImpl.cpp
    src/SyriusEngine/EngineConfiguration.cpp)

add_library(SyriusEngine SHARED ${SR_ENGINE_SRC})
add_dependencies(SyriusEngine CopyDLL)
target_link_libraries(SyriusEngine -lSyriusCore -lSRSLCompiler)


################# APP EXECUTABLE #################
link_directories(./cmake-build-debug/)

include_directories(./Dependencies/assimp/include/)

set(APP_SRC
    SyriusEngineApp/Applayer.cpp
    SyriusEngineApp/Camera.cpp
    SyriusEngineApp/SceneObjects/Mesh.cpp
    SyriusEngineApp/PanelsUI/MeshPanel.cpp
    SyriusEngineApp/PanelsUI/LightPanel.cpp
    SyriusEngineApp/PanelsUI/LightPanel.hpp
    SyriusEngineApp/SceneObjects/LightObject.cpp
    SyriusEngineApp/Serializer/Serializer.cpp
    SyriusEngineApp/Serializer/ModelLoader.cpp
    SyriusEngineApp/Serializer/ModelLoader.hpp
)

add_executable(SyriusEngineApp ${APP_SRC} main.cpp)
add_dependencies(SyriusEngineApp SyriusEngine)

target_link_libraries(SyriusEngineApp -lSyriusEngine -lSyriusCore -lassimp-5)

#################### TESTING ###############################
include_directories(./Dependencies/googletest/googletest/include/)
link_directories(./cmake-build-debug/lib/)

set(TEST_SRC

    Tests/EntityComponentSystem/EntityComponentSystemTest.cpp
    Tests/EntityComponentSystem/ExampleComponents.cpp

    Tests/Utils/WorkerTest.cpp

    src/SyriusEngine/Renderer/RenderGraph/RenderGraphDefs.hpp)

add_executable(SyriusEngineTest TestMain.cpp ${SR_ENGINE_SRC} ${TEST_SRC})
add_dependencies(SyriusEngineTest gtest)
target_link_libraries(SyriusEngineTest -lSyriusCore -lgtest)
