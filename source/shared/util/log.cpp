#include "log.h"

#pragma warning(push, 0)
#include <fmt/color.h>
#include <fmt/chrono.h>
#pragma warning(pop)

bool bLogInitialized = false;

#include <time.h>
#include <mutex>
#include <fstream>
#include <filesystem>

// from: minwindef.h
#define MAX_PATH 260

std::mutex LogFileLock;
std::ofstream LogFile;

void LogInit(const char* cLogName, const char* cLogLocation) {
    if (bLogInitialized)
        return;

    std::string sLogFilename = fmt::format("{} - {:%Y-%m-%d %H.%M.%S}.txt", cLogName, fmt::localtime(std::time(nullptr)));

    char cLogPath[MAX_PATH];
    sprintf(cLogPath, "%s%s", cLogLocation, sLogFilename.c_str());

    std::filesystem::create_directories(cLogLocation);

    LogFile.open(cLogPath, std::ios::out);
    bLogInitialized = true;
}

void Log(ELogType eLogType, std::string sText, bool bNewline) {
    fmt::color LogColor = fmt::color::white;
    std::string sLogType;

    switch (eLogType)
    {
    case ELogType::Verbose:
        sLogType = "VERBOSE";
        LogColor = fmt::color::green;
        break;
    case ELogType::Info:
        sLogType = "INFO";
        LogColor = fmt::color::blue;
        break;
    case ELogType::Warning:
        sLogType = "WARNING";
        LogColor = fmt::color::yellow;
        break;
    case ELogType::Error:
        sLogType = "ERROR";
        LogColor = fmt::color::red;
        break;
    case ELogType::Net:
        sLogType = "C#";
        LogColor = fmt::color::orange;
        break;
    case ELogType::NetWarning:
        sLogType = "C# WARNING";
        LogColor = fmt::color::light_yellow;
        break;
    case ELogType::NetError:
        sLogType = "C# ERROR";
        LogColor = fmt::color::dark_red;
        break;
    case ELogType::NetVerbose:
        sLogType = "C# VERBOSE";
        LogColor = fmt::color::dark_green;
        break;
    case ELogType::Teardown:
        sLogType = "TEARDOWN";
        LogColor = fmt::color::brown;
        break;
    }

    if (bLogInitialized) {
        LogFileLock.lock();
        LogFile << fmt::format("{:%Y-%m-%d - %H:%M:%S} - [{}] - {}", fmt::localtime(std::time(nullptr)), sLogType, sText) << std::endl;
        LogFileLock.unlock();
    }

    #ifndef _DEBUG
        if (eLogType == ELogType::Verbose || eLogType == ELogType::NetVerbose) return;
    #endif

    fmt::print(fg(LogColor), "[{}]", sLogType);
    if (bNewline)
        fmt::print(fg(fmt::color::white), " {}\n", sText);
    else
        fmt::print(fg(fmt::color::white), " {}", sText);

 }
