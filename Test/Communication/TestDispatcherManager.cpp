#include <gtest/gtest.h>

#include "../../src/Communication/DispatcherManager.hpp"

using namespace Syrius;

struct DummyData {
    int x = 0;
    int y = 0;

    DummyData() = default;

    DummyData(int x_, int y_) : x(x_), y(y_) {}
};

struct DummyDataReceiver {

    std::unordered_map<UID, SP<DummyData>> receivedData;

    void onCreate(const UID uid, SP<DummyData> data) {
        receivedData[uid] = data;
    }

    void onUpdate(const UID uid, SP<DummyData> data) {
        receivedData[uid] = data;
    }

    void onDelete(const UID uid) {
        receivedData.erase(uid);
    }
};

class TestDispatcherManager: public ::testing::Test {
protected:

    void SetUp() override {

    }

    void TearDown() override {

    }
};

TEST_F(TestDispatcherManager, dispatch_dummy_data) {
    auto d1 = std::make_shared<DummyData>(1, 2);
    auto d2 = std::make_shared<DummyData>(3, 4);

    DummyDataReceiver receiver;

    // Setup and register functions
    DispatcherManager manager(createSP<WorkerPool>());
    auto dummyDataDispatcher = manager.getDispatcher<UID, DummyData>();
    dummyDataDispatcher->registerCreate([&](const UID uid, SP<DummyData> data) {
        receiver.onCreate(uid, data);
    });
    dummyDataDispatcher->registerUpdate([&](const UID uid, SP<DummyData> data) {
        receiver.onUpdate(uid, data);
    });
    dummyDataDispatcher->registerDelete([&](UID uid) {
        receiver.onDelete(uid);
    });

    // Dispatch the create
    dummyDataDispatcher->dispatchCreate(1, d1);
    dummyDataDispatcher->dispatchCreate(2, d2);

    std::this_thread::sleep_for(1ms);

    // Check that the receiver received the data
    EXPECT_EQ(receiver.receivedData.size(), 2);
    EXPECT_EQ(receiver.receivedData[1]->x, 1);
    EXPECT_EQ(receiver.receivedData[1]->y, 2);
    EXPECT_EQ(receiver.receivedData[2]->x, 3);
    EXPECT_EQ(receiver.receivedData[2]->y, 4);

    // Now Update!
    auto d1_updated = std::make_shared<DummyData>(5, 6);
    auto d2_updated = std::make_shared<DummyData>(7, 8);
    dummyDataDispatcher->dispatchUpdate(1, d1_updated);
    dummyDataDispatcher->dispatchUpdate(2, d2_updated);

    std::this_thread::sleep_for(1ms);
    EXPECT_EQ(receiver.receivedData[1]->x, 5);
    EXPECT_EQ(receiver.receivedData[1]->y, 6);
    EXPECT_EQ(receiver.receivedData[2]->x, 7);
    EXPECT_EQ(receiver.receivedData[2]->y, 8);

}
