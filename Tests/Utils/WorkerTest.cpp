#include "WorkerTest.hpp"

void WorkerTest::SetUp() {
    Test::SetUp();
}

void WorkerTest::TearDown() {
    Test::TearDown();
}

TEST_F(WorkerTest, CreateWorker){
    const std::string name = "TestWorker";
    Worker worker(name);

    EXPECT_EQ(worker.getName(), name);
}