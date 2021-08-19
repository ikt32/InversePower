#include "Logger.hpp"

#include <Windows.h>

#include <fstream>
#include <iomanip>

namespace {
    const std::vector<std::string> levelStrings{
        " DEBUG ",
        " INFO  ",
        "WARNING",
        " ERROR ",
        " FATAL ",
    };
}

std::string Log::mFile = "";
LogLevel Log::mMinLevel = INFO;

void Log::SetFile(const std::string &fileName) {
    mFile = fileName;
}

void Log::SetMinLevel(LogLevel level) {
    mMinLevel = level;
}

void Log::Clear() {
    std::ofstream logFile(mFile, std::ofstream::out | std::ofstream::trunc);
}

void Log::Write(LogLevel level, const std::string& text) {
    if (level < mMinLevel) return;
    std::ofstream logFile(mFile, std::ios_base::out | std::ios_base::app);
    SYSTEMTIME currTimeLog;
    GetLocalTime(&currTimeLog);
    logFile << "[" <<
        std::setw(2) << std::setfill('0') << currTimeLog.wHour << ":" <<
        std::setw(2) << std::setfill('0') << currTimeLog.wMinute << ":" <<
        std::setw(2) << std::setfill('0') << currTimeLog.wSecond << "." <<
        std::setw(3) << std::setfill('0') << currTimeLog.wMilliseconds << "] " <<
        "[" << getLevelText(level) << "] " <<
        text << "\n";
}

std::string Log::getLevelText(LogLevel level) {
    return levelStrings[level];
}
