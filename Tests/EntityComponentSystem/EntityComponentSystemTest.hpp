#pragma once

#include "../TestInclude.hpp"
#include "../../include/SyriusEngine/EntityComponentSystem/EntityComponentSystem.hpp"
#include "ExampleComponents.hpp"


class EntityComponentSystemTest : public ::testing::Test{
protected:
    void SetUp() override;

    void TearDown() override;
};