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
	Teardown
};

enum class ELogLevel {
	Verbose,
	Info
};

#ifdef _DEBUG
inline ELogLevel LogLevel = ELogLevel::Verbose;
#else
inline ELogLevel LogLevel = ELogLevel::Info;
#endif

template <typename... Args>
inline std::string FormatLog(const char* cFmt, Args&&... Arguments) {
	return fmt::vformat(cFmt, fmt::make_format_args(std::forward<Args>(Arguments)...));
}

void Log(ELogType eLogType, std::string sMessage, bool bNewline = true);

template<typename... Args>
inline void LogVerbose(const char* cFmt, Args... Arguments) { Log(ELogType::Verbose, FormatLog(cFmt, std::forward<Args>(Arguments)...)); }

template<typename... Args>
inline void LogInfo(const char* cFmt, Args... Arguments) { Log(ELogType::Info, FormatLog(cFmt, std::forward<Args>(Arguments)...)); }

template<typename... Args>
inline void LogWarning(const char* cFmt, Args... Arguments) { Log(ELogType::Warning, FormatLog(cFmt, std::forward<Args>(Arguments)...)); }

template<typename... Args>
inline void LogError(const char* cFmt, Args... Arguments) { Log(ELogType::Error, FormatLog(cFmt, std::forward<Args>(Arguments)...)); }