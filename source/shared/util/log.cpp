 #include "log.h"
 #include <fmt/color.h>

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
 }
