#include "Tests/EngineInitializationTest.hpp"

class TestDebugMessageHandler{
public:
    static void init(const std::string& debugOutput){
        m_File = std::ofstream(debugOutput);
        setDebugMessageHandler(TestDebugMessageHandler::messageCallback);
    }

    static void terminate(){
    }

    static void messageCallback(const EngineMessage& msg){
        std::string severity;
        switch (msg.severity){
            case SR_MESSAGE_SEVERITY_INFO:
                severity = "INFO";
                break;
            case SR_MESSAGE_SEVERITY_LOW:
                severity = "LOW";
                break;
            case SR_MESSAGE_SEVERITY_MEDIUM:
                severity = "MEDIUM";
                break;
            case SR_MESSAGE_SEVERITY_HIGH:
                severity = "HIGH";
                break;
        }

        std::string source;
        switch (msg.messageType){
            case SR_MESSAGE:
                source = "MESSAGE";
                break;
            case SR_MESSAGE_SYRIUS_CORE:
                source = "SYRIUS_CORE";
                break;
            case SR_MESSAGE_RENDERER:
                source = "RENDERER";
                break;
            case SR_MESSAGE_PRECONDITION:
                source = "PRECONDITION";
                break;
            case SR_MESSAGE_POSTCONDITION:
                source = "POSTCONDITION";
                break;
            case SR_MESSAGE_ASSERTION:
                source = "ASSERTION";
                break;
        }
        auto message = msg.message;
        message.erase(remove(message.begin(), message.end(), '\n'), message.end());
        std::string final = "[" + msg.file + " : " + msg.function + " : " + std::to_string(msg.line) + "][" + source + " : " + severity + "]: " + message + "\n";

        m_File << final;
    }

private:
    static std::ofstream m_File;
};

std::ofstream TestDebugMessageHandler::m_File;

int main(int argc, char** argv) {
    TestDebugMessageHandler::init("TestMessageOutput.txt");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
