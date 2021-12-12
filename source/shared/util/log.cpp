#include "log.h"
#include <fmt/color.h>

void Log(ELogType eLogType, std::string sText) {
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
	}

	fmt::print(fg(LogColor), "[{}]", sLogType);
	fmt::print(fg(fmt::color::white), " {}\n", sText);
}