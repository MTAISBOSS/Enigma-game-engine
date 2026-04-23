#include "Logger.h"
#include <ctime>
#include <chrono>
#include <string>
#include <iostream>

#ifdef _WIN32

#include <windows.h>

std::vector<LogEntry> Logger::messages;

void Logger::EnableWindowsANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
    if (hErr != INVALID_HANDLE_VALUE && GetConsoleMode(hErr, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hErr, dwMode);
    }
}

#endif

std::string CurrentDateTimeToString() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');

    size_t length = std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));

    output.resize(length);

    return output;
}

void Logger::Log(const std::string &message) {
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG :[" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\033[32m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}

void Logger::LogError(const std::string &message) {
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERROR :[" + CurrentDateTimeToString() + "]: " + message;
    std::cerr << "\033[91m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}
