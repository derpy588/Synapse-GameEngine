#include <gtest/gtest.h>

#include "SynapseEngine/Core/Logger.h"

TEST(ExampleTest, DoesNothing) { EXPECT_TRUE(true); }

TEST(LoggerTest, LogLevels) {

    EXPECT_NO_THROW({
        Synapse::Logger::getInstance().addSink(std::make_shared<Synapse::ConsoleSink>());
        Synapse::Logger::getInstance().addSink(std::make_shared<Synapse::FileSink>("test_log.txt"));

        Synapse::Logger::getInstance().setGlobalLevel(Synapse::LogLevel::Trace);

        ENGINE_LOG_TRACE("This is a trace message", "Test");
        ENGINE_LOG_DEBUG("This is a debug message", "Test");
        ENGINE_LOG_INFO("This is an info message", "Test");
        ENGINE_LOG_WARNING("This is a warning message", "Test");
        ENGINE_LOG_ERROR("This is an error message", "Test");
        ENGINE_LOG_CRITICAL("This is a critical message", "Test");
    });
}