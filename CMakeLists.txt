cmake_minimum_required(VERSION 3.18)
project(SyriusEngine)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(SR_EXPORT_DLL)
add_compile_definitions(SR_DEBUG)

add_subdirectory(./Dependencies/SRSLCompiler/)
add_subdirectory(./Dependencies/SyriusCore/)

set(SR_DEPENDENCIES_DLL
    ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/SyriusCore/libSyriusCore.dll
    ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/SRSLCompiler/libSRSLCompiler.dll
    )

add_custom_target(CopyDLL ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SR_DEPENDENCIES_DLL} ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${VISU3D_DEPENDECIES_DLL}
    COMMENT "Copying DLLs to build tree")

include_directories(./Dependencies/SyriusCore/include/)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/Dependencies/SyriusCore/)

include_directories(./Dependencies/SRSLCompiler/include/)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/Dependencies/SRSLCompiler/)

include_directories(./Dependencies/glm/)

include_directories(./Dependencies/EasyIni/include/)

set(SR_ENGINE_CORE_SRC
    src/SyriusEngine/Core/DebugMessageHandler.cpp
    src/SyriusEngine/Core/InternalInclude.cpp)

set(SR_ENGINE_SRC
    ${SR_ENGINE_CORE_SRC}
    src/SyriusEngine/SyriusEngine.cpp
    src/SyriusEngine/LayerStack.cpp
    src/SyriusEngine/SyriusEngineImpl.cpp)

add_library(SyriusEngine SHARED ${SR_ENGINE_SRC})
add_dependencies(SyriusEngine CopyDLL)
target_link_libraries(SyriusEngine -lSyriusCore -lSRSLCompiler)


################# TEST EXECUTABLE #################
link_directories(./cmake-build-debug/)
add_executable(SyriusEngineDev main.cpp)

target_link_libraries(SyriusEngineDev -lSyriusEngine -lSyriusCore)
