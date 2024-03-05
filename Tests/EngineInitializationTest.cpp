#include "EngineInitializationTest.hpp"

void EngineInitializationTest::SetUp() {

}

void EngineInitializationTest::TearDown() {

}

TEST_F(EngineInitializationTest, engineStart){
    std::string configFile = "./Config.ini";
    try{
        auto engine = createEngine(configFile);
        EXPECT_TRUE(engine.get() != nullptr);
    } catch(...){
        EXPECT_TRUE(false); // should not happen, thus assert false
    }
}

TEST_F(EngineInitializationTest, nonExistingConfigFile){
    std::string configFile = "./NON_EXISTING.ini";
    try{
        auto engine = createEngine(configFile);
        EXPECT_TRUE(engine.get() != nullptr);
    } catch(std::exception& e){
        std::string exc = e.what();
        EXPECT_TRUE(exc == "File: " + configFile + " does not exist");
    }
}
