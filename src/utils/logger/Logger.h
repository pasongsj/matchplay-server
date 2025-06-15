#pragma once

#include <string>
#include <mutex>
#include <format>    // C++20 이상
#include <iostream>
#include <chrono>
#include <ctime>

enum class LogLevel {
    TRACE, DEBUG, INFO, WARN, ERR, FATAL
};

enum class LogMode {
    DEFAULT,  // INFO 이상
    DEBUG,    // DEBUG 이상
    TRACE     // TRACE 이상
};

class Logger {
public:
    static Logger& Instance();

    void SetMode(LogMode mode);
    void Log(LogLevel level, const std::string& message);

    template<typename... Args>
    void Log(LogLevel level, const std::string& fmt, Args&&... args) {
        if (!ShouldLog(level)) return;

        std::string formatted = std::vformat(fmt, std::make_format_args(args...));
        Log(level, formatted);
    }

private:
    Logger() = default;
    LogMode m_mode = LogMode::DEFAULT;
    std::mutex m_mutex;

    bool ShouldLog(LogLevel level);
    std::string LevelToString(LogLevel level);
    std::string GetTimestamp();
};
