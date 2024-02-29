#include <iostream>
#include "../../../include/SyriusEngine/Core/DebugMessageHandler.hpp"

namespace Syrius{

    HandleDebugMessageFunc DebugMessageHandler::m_MessageHandler = DebugMessageHandler::defaultMessageHandler;

    void DebugMessageHandler::defaultMessageHandler(const EngineMessage &msg) {
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
        if (msg.severity == SR_MESSAGE_SEVERITY_HIGH){
            std::cerr << "[" << msg.file << " : " << msg.function << " : " << msg.line << "]" << "[" << source << " : " << severity << "]: " << msg.message << std::endl;
        }
        else{
            std::cout << "[" << msg.file << " : " << msg.function << " : " << msg.line << "]" << "[" << source << " : " << severity << "]: " << msg.message << std::endl;
        }
    }

    void DebugMessageHandler::receiveSyriusCoreMessage(const Syrius::Message &msg) {
        EngineMessage message;
        message.messageType = SR_MESSAGE_SYRIUS_CORE;
        message.severity = SR_MESSAGE_SEVERITY_INFO;
        switch (msg.severity){
            case SR_CORE_MESSAGE_SEVERITY_INFO: message.severity = SR_MESSAGE_SEVERITY_INFO; break;
            case SR_CORE_MESSAGE_SEVERITY_LOW: message.severity = SR_MESSAGE_SEVERITY_LOW; break;
            case SR_CORE_MESSAGE_SEVERITY_MEDIUM: message.severity = SR_MESSAGE_SEVERITY_MEDIUM; break;
            case SR_CORE_MESSAGE_SEVERITY_HIGH: message.severity = SR_MESSAGE_SEVERITY_HIGH; break;
        }
        std::string srcSource = getMessageTypeString(msg.messageType);
        message.message = "[" + srcSource + "]: " + msg.message;
        message.function = msg.function;
        message.file = msg.file;
        message.line = msg.line;

        if (m_MessageHandler != nullptr){
            m_MessageHandler(message);
        }
        else{
            defaultMessageHandler(message);
        }

    }

}