#include "Common.h"
#include "Logger.h"

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

void Logger::SetMode(LogMode mode) {
    m_mode = mode;
}

bool Logger::ShouldLog(LogLevel level) {
    switch (m_mode) {
    case LogMode::DEFAULT:
        return level >= LogLevel::INFO;
    case LogMode::DEBUG:
        return level >= LogLevel::DEBUG;
    case LogMode::TRACE:
        return true;
    }
    return false;
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::TRACE: return "TRACE";
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO:  return "INFO";
    case LogLevel::WARN:  return "WARN";
    case LogLevel::ERR: return "ERR";
    case LogLevel::FATAL: return "FATAL";
    }
    return "UNKNOWN";
}

std::string Logger::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    char buf[32];
    ctime_s(buf, sizeof(buf), &t);
    buf[strcspn(buf, "\n")] = '\0';
    return std::string(buf);
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (!ShouldLog(level)) return;

    std::lock_guard lock(m_mutex);
    std::cout << "[" << GetTimestamp() << "] "
        << "[" << LevelToString(level) << "] "
        << message << std::endl;
}
