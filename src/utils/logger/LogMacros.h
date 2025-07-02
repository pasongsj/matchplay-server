#pragma once
#include <utils/logger/Logger.h>

#define LOG_TRACE(fmt, ...) Logger::GetInstance().Log(LogLevel::TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) Logger::GetInstance().Log(LogLevel::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  Logger::GetInstance().Log(LogLevel::INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  Logger::GetInstance().Log(LogLevel::WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...)   Logger::GetInstance().Log(LogLevel::ERR,   __FILE__, __LINE__, fmt, ##__VA_ARGS__)
