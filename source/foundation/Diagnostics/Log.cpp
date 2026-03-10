#include "foundation/Diagnostics/Log.h"

#include <fmt/core.h>

#include <iostream>

namespace Alice::Foundation::Diagnostics
{
    namespace
    {
        const char* ToString(const LogLevel level)
        {
            switch (level)
            {
            case LogLevel::Info:
                return "INFO";
            case LogLevel::Warning:
                return "WARN";
            case LogLevel::Error:
                return "ERROR";
            default:
                return "UNKNOWN";
            }
        }
    }

    void WriteLog(const LogLevel level, const std::string_view message)
    {
        std::cout << fmt::format("[{}] {}\n", ToString(level), message);
    }
}
