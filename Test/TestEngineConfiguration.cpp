#include <gtest/gtest.h>

#include "../include/SyriusEngine/EngineConfiguration.hpp"

using namespace Syrius;

TEST(EngineConfigurationTest, DefaultConstructor) {
    const std::vector<std::string_view> args = {
        "-api", "OpenGL",
        "-width", "800",
        "-height", "600",
        "-vsync"
    };
    EngineConfiguration config(args);
    EXPECT_EQ(config.api, SR_API_OPENGL);
    EXPECT_EQ(config.windowWidth, 800);
    EXPECT_EQ(config.windowHeight, 600);
    EXPECT_TRUE(config.vsync);
}
