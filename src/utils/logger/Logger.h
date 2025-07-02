// Logger.h
#pragma once

#include <string>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <format>
#include <mutex>
#include <utils/ThreadUtil.h>
#include <magic_enum/magic_enum.hpp>

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERR,
    UNKNOWN,
};

class Logger {
public:
    static Logger& GetInstance();

    void SetProjectRoot(const std::string& root);
    void SetLogLevel(LogLevel level);

    template <typename... Args>
    void Log(LogLevel level, const char* file, int line, std::format_string<Args...> fmt, Args&&... args) {
        if (static_cast<int>(level) < static_cast<int>(currentLevel_)) return;

        std::string formatted;
        try {
            formatted = std::format(fmt, std::forward<Args>(args)...);
        }
        catch (const std::format_error& e) {
            formatted = std::format("[format_error: {}] {}", e.what(), fmt.get());
        }

        LogItem item;
        item.timestamp = GetTimestamp();
        item.levelStr = std::string(magic_enum::enum_name(level));
        item.fileLine = std::format("{}:{}", TrimToRelativePath(file), line);
        item.message = std::move(formatted);

        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            logQueue_.push(std::move(item));
        }
        cv_.notify_one();
    }

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void ConsumeLogs();
    std::string TrimToRelativePath(const char* fullPath) const;
    std::string GetTimestamp() const;

    struct LogItem {
        std::string timestamp;
        std::string levelStr;
        std::string fileLine;
        std::string message;
    };

    std::queue<LogItem> logQueue_;
    std::mutex queueMutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_ = true;

    std::string projectRoot_;
    LogLevel currentLevel_ = LogLevel::INFO;
};
