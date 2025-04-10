#pragma once

#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <fstream>

#define ENGINE_LOG_TRACE(message, category) \
    Synapse::Logger::trace(message, category, __FILE__, __LINE__, __FUNCTION__)

#define ENGINE_LOG_DEBUG(message, category) \
    Synapse::Logger::debug(message, category, __FILE__, __LINE__, __FUNCTION__)

#define ENGINE_LOG_INFO(message, category) \
    Synapse::Logger::info(message, category, __FILE__, __LINE__, __FUNCTION__)

#define ENGINE_LOG_WARNING(message, category) \
    Synapse::Logger::warning(message, category, __FILE__, __LINE__, __FUNCTION__)

#define ENGINE_LOG_ERROR(message, category) \
    Synapse::Logger::error(message, category, __FILE__, __LINE__, __FUNCTION__)

#define ENGINE_LOG_CRITICAL(message, category) \
    Synapse::Logger::critical(message, category, __FILE__, __LINE__, __FUNCTION__)

namespace Synapse {
    /**
     * 
     * Simple logging levels types in an Enum
     * 
     * Trace is for detailed information
     * Info is for general information
     * Warn is for warning messages
     * Error is for error messages
     * Debug is for debugging information
     * Critical is for critical error messages
     * Off is to disable logging
     * 
     */
    enum class LogLevel {
        Trace, Debug, Info, Warning, Error, Critical, Off
    };


    /**
     * 
     * LogMessage Parameters:
     * 
     * Level is what level the log message is
     * message is the log message itself
     * category is for what system like renderer, physics, etc.
     * file is the source file where the log was generated
     * line is the line number in the source file
     * function is the function name where the log was generated
     * threadId is the ID of the thread that generated the log
     * timestamp is the time when the log was generated
     * 
     */
    struct LogMessage {
        LogLevel level;
        std::string message;
        std::string category;
        std::string file;
        int line;
        std::string function;
        std::thread::id threadId;
        std::chrono::system_clock::time_point timestamp;

    };

    /**
     * 
     * Sink interface for logging
     * 
     * This interface is used to create different logging sinks
     * like console, file, network, etc.
     * 
     */
    class ILogSink {
        public:
            virtual ~ILogSink() = default;
            virtual void write(const LogMessage& message) = 0;
            virtual void flush() = 0;
    };


    /**
     * 
     * The main Logger class
     * Highly suggest you use the macros provided for logging
     * 
     */
    class Logger {

        private:
            LogLevel globalLevel = LogLevel::Info;
            std::vector<std::shared_ptr<ILogSink>> sinks;
            std::mutex logMutex;
            std::unordered_map<std::string, LogLevel> categoryLevels;

            

        public:
            static Logger& getInstance() {
                static Logger instance;
                return instance;
            }

            void addSink(std::shared_ptr<ILogSink> sink);
            void setGlobalLevel(LogLevel level);
            void setCategoryLevel(const std::string& category, LogLevel level);
            bool shouldLog(LogLevel level, const std::string& category);
            void log(LogLevel level, const std::string& message,
                     const std::string& category,
                     const std::string& file,
                     int line,
                     const std::string& function);

            static void trace(const std::string& message, const std::string& category,
                            const std::string& file, int line, const std::string& function) {

                getInstance().log(LogLevel::Trace, message, category, file, line, function);
            }

            static void debug(const std::string& message, const std::string& category,
                            const std::string& file, int line, const std::string& function)
            {
                getInstance().log(LogLevel::Debug, message, category, file, line, function);
            }

            static void info(const std::string& message, const std::string& category,
                            const std::string& file, int line, const std::string& function)
            {
                getInstance().log(LogLevel::Info, message, category, file, line, function);
            }

            static void warning(const std::string& message, const std::string& category,
                            const std::string& file, int line, const std::string& function)
            {
                getInstance().log(LogLevel::Warning, message, category, file, line, function);
            }

            static void error(const std::string& message, const std::string& category,
                            const std::string& file, int line, const std::string& function)
            {
                getInstance().log(LogLevel::Error, message, category, file, line, function);
            }

            static void critical(const std::string& message, const std::string& category,
                            const std::string& file, int line, const std::string& function)
            {
                getInstance().log(LogLevel::Critical, message, category, file, line, function);
            }

    };


    class ConsoleSink : public ILogSink {
        public:
            void write(const LogMessage& message) override;
            void flush() override;

        private:
            void setConsoleColor(LogLevel level);
            void resetConsoleColor();
            std::string formatMessage(const LogMessage& message);
    };


    class FileSink : public ILogSink {
        public:
            FileSink(const std::string& filename);
            ~FileSink();
            void write(const LogMessage& message) override;
            void flush() override;

        private:
            std::string formatMessage(const LogMessage& message);

        private:
            std::string filename;
            std::ofstream fileStream;
    };
}