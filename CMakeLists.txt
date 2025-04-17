cmake_minimum_required(VERSION 3.16)
project(SyriusEngine)

set(CMAKE_CXX_STANDARD 17)

include(SyriusBuildUtils/CMakeUtils.cmake)
include(cmake/FetchLibs.cmake)

include_or_fetch(SyriusUtils SYRIUS_UTILS_DIR)
message(STATUS "SyriusUtils found in ${SYRIUS_UTILS_DIR}")
include_or_fetch(SyriusCore SYRIUS_CORE_DIR)
message(STATUS "SyriusCore found in ${SYRIUS_CORE_DIR}")

set(CMAKE_POSITION_INDEPENDENT_CODE ON) # Needed to link this static lib into a dynamic lib

set(SR_ENGINE_HEADERS
    include/SyriusEngine/SyriusEngine.hpp
    include/SyriusEngine/EngineConfiguration.hpp

    include/SyriusEngine/Renderer/RenderPrimitives.hpp
    include/SyriusEngine/Utils/EngineInclude.hpp
    include/SyriusEngine/Layer.hpp
)

set(SR_ENGINE_SRC
    src/EngineConfiguration.cpp
    src/SyriusEngine.cpp
    src/LayerStack.cpp
    src/LayerStack.hpp
    src/EngineData.cpp
    src/EngineData.hpp
    src/Communication/AccessFunctions.hpp
    src/Communication/Dispatcher.hpp
    src/Communication/DispatcherManager.hpp
    src/Utils/Worker.cpp
    src/Utils/Worker.hpp
)

add_library(SyriusEngine SHARED ${SR_ENGINE_SRC} ${SR_ENGINE_HEADERS})
target_include_directories(SyriusEngine PUBLIC include)

set(SR_ENGINE_LIBRARIES
    SyriusUtils
    SyriusCore
    glm::glm
)

target_link_libraries(SyriusEngine PUBLIC ${SR_ENGINE_LIBRARIES})
add_dependencies(SyriusEngine glm)

set(SR_ENGINE_TEST_SRC
    Test/TestEngineConfiguration.cpp
    Test/Communication/TestDispatcherManager.cpp)

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTS)
    enable_testing()

    message(STATUS "Building tests")

    add_executable(TestSyriusEngine ${SR_ENGINE_TEST_SRC})
    add_dependencies(TestSyriusEngine gtest)

    target_link_libraries(TestSyriusEngine SyriusEngine GTest::gtest GTest::gtest_main)

    add_test(NAME CTestSyriusEngine COMMAND ${PROOT} $<TARGET_FILE:TestSyriusEngine> OpenGL)
    set_tests_properties(CTestSyriusEngine PROPERTIES
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

endif ()

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_APP)
    add_executable(SyriusEngineApp main.cpp)
    target_link_libraries(SyriusEngineApp PUBLIC SyriusEngine)
endif ()