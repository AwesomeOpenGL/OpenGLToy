#pragma once

#include <string_view>

namespace Alice::Foundation::Diagnostics
{
    [[noreturn]] void HandleAssertFailure(const char* expression, const char* file, int line, std::string_view message);
}

#define LEOCPP_ASSERT(condition, message) \
    do \
    { \
        if (!(condition)) \
        { \
            ::Alice::Foundation::Diagnostics::HandleAssertFailure(#condition, __FILE__, __LINE__, (message)); \
        } \
    } while (false)
