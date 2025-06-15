#pragma once
#include "Logger.h"

#define LOG_TRACE(fmt, ...) Logger::Instance().Log(LogLevel::TRACE, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) Logger::Instance().Log(LogLevel::DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  Logger::Instance().Log(LogLevel::INFO,  fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  Logger::Instance().Log(LogLevel::WARN,  fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) Logger::Instance().Log(LogLevel::ERR, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) Logger::Instance().Log(LogLevel::FATAL, fmt, ##__VA_ARGS__)
