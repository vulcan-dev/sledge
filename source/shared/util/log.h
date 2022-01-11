#pragma once

#include <fmt/format.h>

enum class ELogType {
	Verbose,
	Info,
	Warning,
	Error,
	Net,
	NetWarning,
	NetError,
	NetVerbose,
	Teardown
};

template <typename... Args>
inline std::string FormatLog(const char* cFmt, Args&&... Arguments) {
	return fmt::vformat(cFmt, fmt::make_format_args(std::forward<Args>(Arguments)...));
}

void Log(ELogType eLogType, std::string sMessage, bool bNewline = true);

void LogInit(const char* cLogName, const char* cLogLocation);

template<typename... Args>
inline void LogVerbose(const char* cFmt, Args... Arguments) { Log(ELogType::Verbose, FormatLog(cFmt, std::forward<Args>(Arguments)...)); }

template<typename... Args>
inline void LogInfo(const char* cFmt, Args... Arguments) { Log(ELogType::Info, FormatLog(cFmt, std::forward<Args>(Arguments)...)); }

template<typename... Args>
inline void LogWarning(const char* cFmt, Args... Arguments) { Log(ELogType::Warning, FormatLog(cFmt, std::forward<Args>(Arguments)...)); }

template<typename... Args>
inline void LogError(const char* cFmt, Args... Arguments) { Log(ELogType::Error, FormatLog(cFmt, std::forward<Args>(Arguments)...)); }