#pragma once

#include "PlatformDetection.hpp"
#include <string>
#include <vector>
#include <exception>

#include <SyriusCore/SyriusCore.hpp>

class SyriusEngineException : public std::exception{
public:
    explicit SyriusEngineException(const std::string& message): m_Message(message){

    }

    [[nodiscard]] const char* what() const noexcept override{
        return m_Message.c_str();
    }

private:
    std::string m_Message;
};

typedef enum SR_MESSAGE_SOURCE{
    SR_MESSAGE                  = 0x00,
    SR_MESSAGE_SYRIUS_CORE      = 0x01,
    SR_MESSAGE_RENDERER         = 0x02,

    SR_MESSAGE_PRECONDITION     = 0x1001,
    SR_MESSAGE_POSTCONDITION    = 0x1002,
    SR_MESSAGE_ASSERTION        = 0x1003,
} SR_MESSAGE_SOURCE;

typedef enum SR_MESSAGE_SEVERITY{
    SR_MESSAGE_SEVERITY_INFO    = 0x00,
    SR_MESSAGE_SEVERITY_LOW     = 0x01,      // mostly used for messages that are not important
    SR_MESSAGE_SEVERITY_MEDIUM  = 0x02,      // used for messages that are important, but not critical
    SR_MESSAGE_SEVERITY_HIGH    = 0x03,      // used for messages that are critical and (usually) results in program termination
} SR_MESSAGE_SEVERITY;

namespace Syrius{

    struct EngineMessage{
        SR_MESSAGE_SOURCE messageType;
        SR_MESSAGE_SEVERITY severity;
        std::string message;
        std::string function;
        std::string file;
        uint32_t line;
    };

    typedef void(*HandleDebugMessageFunc)(const EngineMessage&);

    class SR_API DebugMessageHandler{
    public:
        template<typename... Args>
        static void log(
                SR_MESSAGE_SOURCE messageType,
                SR_MESSAGE_SEVERITY severity,
                const char* function,
                const char* file,
                uint32_t line,
                const char* message,
                Args... args
        ){
            uint64_t formatSize = std::snprintf(nullptr, 0, message, args...);
            std::vector<uint8_t> formatBuffer(formatSize + 1);
            std::snprintf(reinterpret_cast<char*>(formatBuffer.data()), formatSize + 1, message, args...);

            EngineMessage msg;
            msg.messageType = messageType;
            msg.severity = severity;
            msg.function = function;
            msg.file = file;
            msg.line = line;
            msg.message = std::string(reinterpret_cast<char*>(formatBuffer.data()));

            if (m_MessageHandler != nullptr){
                m_MessageHandler(msg);
            }
            else{
                defaultMessageHandler(msg);
            }
        }

        template<typename... Args>
        static void engineAssert(
                SR_MESSAGE_SOURCE messageType,
                const char* condition,
                const char* function,
                const char* file,
                uint32_t line,
                const char* message,
                Args... args
        ){
            uint64_t formatSize = std::snprintf(nullptr, 0, message, args...);
            std::vector<uint8_t> formatBuffer(formatSize + 1);
            std::snprintf(reinterpret_cast<char*>(formatBuffer.data()), formatSize + 1, message, args...);

            EngineMessage msg;
            msg.messageType = messageType;
            msg.severity = SR_MESSAGE_SEVERITY_HIGH;
            msg.function = function;
            msg.file = file;
            msg.line = line;
            msg.message = std::string("Condition: ") + condition + ", Message = " + std::string(reinterpret_cast<char*>(formatBuffer.data()));

            if (m_MessageHandler != nullptr){
                m_MessageHandler(msg);
            }
#if defined(__MINGW64__)
            _assert(msg.message.c_str(), file, line);
#else
            assert(msg.message.c_str());
#endif
        }

        template<typename... Args>
        static void engineThrow(
                const char* function,
                const char* file,
                uint32_t line,
                const char* message,
                Args... args
        ){
            uint64_t formatSize = std::snprintf(nullptr, 0, message, args...);
            std::vector<uint8_t> formatBuffer(formatSize + 1);
            std::snprintf(reinterpret_cast<char*>(formatBuffer.data()), formatSize + 1, message, args...);

            std::string msg = "[FATAL ERROR]: " + std::string(reinterpret_cast<char*>(formatBuffer.data()));
            msg += "\n\n at " + std::string(file) + ":" + std::string(function) + ":" + std::to_string(line);
            EngineMessage engineMsg;
            engineMsg.messageType = SR_MESSAGE;
            engineMsg.severity = SR_MESSAGE_SEVERITY_HIGH;
            engineMsg.function = function;
            engineMsg.file = file;
            engineMsg.line = line;
            engineMsg.message = msg;
            if (m_MessageHandler != nullptr){
                m_MessageHandler(engineMsg);
            }
            throw SyriusEngineException(msg);
        }

        static void receiveSyriusCoreMessage(const Syrius::Message& msg);

        static void setDebugMessageHandler(HandleDebugMessageFunc cb);

    private:

        static void defaultMessageHandler(const EngineMessage& msg);

    private:
        static HandleDebugMessageFunc m_MessageHandler;

    };

}

#define SR_THROW(message, ...) Syrius::DebugMessageHandler::engineThrow(__FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__);

#if defined(SR_DEBUG)
#define SR_LOG_INFO(message, ...) Syrius::DebugMessageHandler::log(SR_MESSAGE, SR_MESSAGE_SEVERITY_INFO, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__);
#define SR_LOG_WARNING(message, ...) Syrius::DebugMessageHandler::log(SR_MESSAGE, SR_MESSAGE_SEVERITY_MEDIUM, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__);
#define SR_LOG_ERROR(message, ...) Syrius::DebugMessageHandler::log(SR_MESSAGE, SR_MESSAGE_SEVERITY_HIGH, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__);

#define SR_PRECONDITION(condition, message, ...) if(!(condition)){ Syrius::DebugMessageHandler::engineAssert(SR_MESSAGE_PRECONDITION, #condition, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__); }
#define SR_POSTCONDITION(condition, message, ...) if(!(condition)){ Syrius::DebugMessageHandler::engineAssert(SR_MESSAGE_POSTCONDITION, #condition, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__); }
#define SR_ASSERT(condition, message, ...) if(!(condition)){ Syrius::DebugMessageHandler::engineAssert(SR_MESSAGE_ASSERTION, #condition, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__); }

#else

#define SR_LOG_INFO(message, ...)
#define SR_LOG_WARNING(message, ...)
#define SR_LOG_ERROR(message, ...)

#define SR_PRECONDITION(condition, message, ...)
#define SR_POSTCONDITION(condition, message, ...)
#define SR_ASSERT(condition, message, ...)

#endif