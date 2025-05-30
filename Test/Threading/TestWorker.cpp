#include <gtest/gtest.h>

#include "../../src/Threading/Worker.hpp"

using namespace Syrius;

class TestWorker: public ::testing::Test {
protected:
    void SetUp() override{}

    void TearDown() override{}
};

TEST_F(TestWorker, simpleQueue) {
    Worker w("testworker");

    bool done = false;
    w.add([&] {
        done = true;
    });

    std::this_thread::sleep_for(1ms);
    EXPECT_TRUE(done);
}