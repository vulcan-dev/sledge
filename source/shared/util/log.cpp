 #include "log.h"
 #include <fmt/color.h>

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
    time_t CurrentTime = time(NULL);
    struct tm* LocalTime = localtime(&CurrentTime);

    char cTime[80];
    strftime(cTime, sizeof(cTime), "%H-%M-%S %d-%m-%y", LocalTime);

    size_t iLogLen = strlen(cLogName) + strlen(cTime);

    char* cLogFileName = reinterpret_cast<char*>(malloc(iLogLen));

    if (cLogFileName == NULL)
        return;

    sprintf(cLogFileName, "%s - %s.txt", cLogName, cTime);
    
    bLogInitialized = true;

    char cLogPath[MAX_PATH];
    sprintf(cLogPath, "%s%s", cLogLocation, cLogFileName);

    std::filesystem::create_directories(cLogLocation);

    LogFile.open(cLogPath, std::ios::out);
    LogVerbose("log path: {}", cLogPath);
}

void Log(ELogType eLogType, std::string sText, bool bNewline) {
#ifndef _DEBUG
    if (eLogType == ELogType::Verbose || eLogType == ELogType::NetVerbose)
        return;
#endif

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

    fmt::print(fg(LogColor), "[{}]", sLogType);
    if (bNewline)
        fmt::print(fg(fmt::color::white), " {}\n", sText);
    else
        fmt::print(fg(fmt::color::white), " {}", sText);

    if (bLogInitialized) {
        LogFileLock.lock();
        LogFile << "[" << sLogType << "] " << sText << std::endl;
        LogFileLock.unlock();
    }
 }
