#pragma once

#include <string_view>

namespace Alice::Foundation::Diagnostics
{
    enum class LogLevel
    {
        Info,
        Warning,
        Error
    };

    void WriteLog(LogLevel level, std::string_view message);
}

#define LEOCPP_LOG_INFO(message)  ::Alice::Foundation::Diagnostics::WriteLog(::Alice::Foundation::Diagnostics::LogLevel::Info, (message))
#define LEOCPP_LOG_WARN(message)  ::Alice::Foundation::Diagnostics::WriteLog(::Alice::Foundation::Diagnostics::LogLevel::Warning, (message))
#define LEOCPP_LOG_ERROR(message) ::Alice::Foundation::Diagnostics::WriteLog(::Alice::Foundation::Diagnostics::LogLevel::Error, (message))
