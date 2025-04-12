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

set(SR_ENGINE_SRC
    src/SyriusEngine.cpp
)

add_library(SyriusEngine SHARED ${SR_ENGINE_SRC})
target_include_directories(SyriusEngine PUBLIC include)
target_link_libraries(SyriusEngine PUBLIC SyriusUtils SyriusCore)

set(SR_ENGINE_TEST_SRC)

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTS)
    enable_testing()

    message(STATUS "Building tests")

    add_executable(TestSyriusEngine ${SR_ENGINE_TEST_SRC} ${SR_ENGINE_SRC})
    add_dependencies(TestSyriusEngine gtest)

    target_link_libraries(TestSyriusEngine  GTest::gtest GTest::gtest_main)

endif ()

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_APP)
    add_executable(SyriusEngineApp main.cpp)
    target_link_libraries(SyriusEngineApp PUBLIC SyriusEngine)
endif ()