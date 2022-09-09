#pragma once
#include <format>
#include <string>
#include <vector>

enum LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};

class Log {
public:
    static void SetFile(const std::string &fileName);
    static void SetMinLevel(LogLevel level);
    static void Clear();
    static void Write(LogLevel level, const std::string& text);

    template <typename... Args>
    static void Write(LogLevel level, const std::string& fmt, Args &&...args) {
        Write(level, std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
    }

private:
    static std::string getLevelText(LogLevel level);

    static std::string mFile;
    static LogLevel mMinLevel;
};
