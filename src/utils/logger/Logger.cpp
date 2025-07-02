#include "Common.h"
#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <filesystem>

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    ThreadUtil::CreateDetachedThread([this]() { ConsumeLogs(); });
}

Logger::~Logger() {
    running_ = false;
    cv_.notify_one();  // wake the thread if it's waiting
}

void Logger::SetProjectRoot(const std::string& root) {
    projectRoot_ = root;
    std::replace(projectRoot_.begin(), projectRoot_.end(), '\\', '/');
    if (!projectRoot_.empty() && projectRoot_.back() != '/') {
        projectRoot_ += '/';
    }
}

void Logger::SetLogLevel(LogLevel level) {
    currentLevel_ = level;
}

void Logger::ConsumeLogs() {
    while (running_) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        cv_.wait(lock, [this] { return !logQueue_.empty() || !running_; });

        while (!logQueue_.empty()) {
            const LogItem& item = logQueue_.front();
            std::cout << std::format("[{}] ({}): {}\n", item.levelStr, item.fileLine, item.message);
            logQueue_.pop();
        }
    }
}

std::string Logger::TrimToRelativePath(const char* fullPath) const {
    std::string path(fullPath);
    std::replace(path.begin(), path.end(), '\\', '/');

    constexpr const char* marker = PROJECT_NAME;
    auto pos = path.find(marker);
    if (pos != std::string::npos) {
        return path.substr(pos + std::strlen(marker));
    }
    return path;
}

std::string Logger::GetTimestamp() const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &now_c);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buffer);
}
