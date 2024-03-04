#pragma once

#include "TestInclude.hpp"

class EngineInitializationTest: public testing::Test{
protected:
    void SetUp() override;

    void TearDown() override;
};