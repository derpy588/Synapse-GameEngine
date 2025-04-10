#include "SynapseEngine/Core/Logger.h"

#include <iostream>
#include <sstream>

namespace Synapse {

    void Logger::addSink(std::shared_ptr<ILogSink> sink) {
        std::lock_guard<std::mutex> lock(logMutex);
        sinks.push_back(sink);
    }

    void Logger::setGlobalLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(logMutex);
        globalLevel = level;
    }

    void Logger::setCategoryLevel(const std::string& category, LogLevel level) {
        std::lock_guard<std::mutex> lock(logMutex);
        categoryLevels[category] = level;
    }

    bool Logger::shouldLog(LogLevel level, const std::string& category) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (level < globalLevel) {
            return false;
        }
        auto it = categoryLevels.find(category);
        if (it != categoryLevels.end() && level < it->second) {
            return false;
        }
        return true;
    }

    void Logger::log(LogLevel level, const std::string& message,
            const std::string& category,
            const std::string& file,
            int line,
            const std::string& function) {

        if (!shouldLog(level, category)) return;

        LogMessage logMessage {
            .level = level,
            .message = message,
            .category = category,
            .file = file,
            .line = line,
            .function = function,
            .threadId = std::this_thread::get_id(),
            .timestamp = std::chrono::system_clock::now()
        };

        std::lock_guard<std::mutex> lock(logMutex);

        for (auto& sink : sinks) {
            sink->write(logMessage);
        }

    }

    void ConsoleSink::write(const LogMessage& message) {
        std::string formattedMessage = formatMessage(message);
        setConsoleColor(message.level);
        std::cout << formattedMessage << std::endl;
        resetConsoleColor();
    }

    void ConsoleSink::flush() {
        std::cout.flush();
    }

    void ConsoleSink::setConsoleColor(LogLevel level) {
        switch (level) {
            case LogLevel::Trace:
                std::cout << "\033[37m"; // White
                break;
            case LogLevel::Debug:
                std::cout << "\033[34m"; // Blue
                break;
            case LogLevel::Info:
                std::cout << "\033[32m"; // Green
                break;
            case LogLevel::Warning:
                std::cout << "\033[33m"; // Yellow
                break;
            case LogLevel::Error:
                std::cout << "\033[31m"; // Red
                break;
            case LogLevel::Critical:
                std::cout << "\033[41m"; // Red background
                break;
            default:
                std::cout << "\033[0m";  // Reset
                break;
        }
    }

    void ConsoleSink::resetConsoleColor() {
        std::cout << "\033[0m";
    }

    std::string ConsoleSink::formatMessage(const LogMessage& message) {
        time_t timestamp = std::chrono::system_clock::to_time_t(message.timestamp);
        std::ostringstream oss;
        oss << "[" << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %X") << "] "
            << "[" << message.threadId << "] "
            << "[" << (message.level == LogLevel::Trace ? "TRACE" :
                       message.level == LogLevel::Debug ? "DEBUG" :
                       message.level == LogLevel::Info ? "INFO" :
                       message.level == LogLevel::Warning ? "WARNING" :
                       message.level == LogLevel::Error ? "ERROR" :
                       message.level == LogLevel::Critical ? "CRITICAL" : "OFF") << "] "
            << "[" << message.file << ":" << message.function << ":" << message.line << "] "
            << "[" << message.category << "] "
            << message.message;

        return oss.str();
    }

    FileSink::FileSink(const std::string& filename) {
        fileStream.open(filename, std::ios::app);
    }

    FileSink::~FileSink() {
        if (fileStream.is_open()) {
            fileStream.close();
        }
    }

    void FileSink::write(const LogMessage& message) {
        if (!fileStream.is_open()) return;
        std::string formattedMessage = formatMessage(message);
        fileStream << formattedMessage << std::endl;
    }

    void FileSink::flush() {
        if (fileStream.is_open()) {
            fileStream.flush();
        }
    }

    std::string FileSink::formatMessage(const LogMessage& message) {
        time_t timestamp = std::chrono::system_clock::to_time_t(message.timestamp);
        std::ostringstream oss;
        oss << "[" << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %X") << "] "
            << "[" << message.threadId << "] "
            << "[" << (message.level == LogLevel::Trace ? "TRACE" :
                       message.level == LogLevel::Debug ? "DEBUG" :
                       message.level == LogLevel::Info ? "INFO" :
                       message.level == LogLevel::Warning ? "WARNING" :
                       message.level == LogLevel::Error ? "ERROR" :
                       message.level == LogLevel::Critical ? "CRITICAL" : "OFF") << "] "
            << "[" << message.file << ":" << message.function << ":" << message.line << "] "
            << "[" << message.category << "] "
            << message.message;

        return oss.str();
    }
}