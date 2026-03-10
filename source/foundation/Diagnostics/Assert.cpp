#include "foundation/Diagnostics/Assert.h"

#include <stdexcept>
#include <string>

namespace Alice::Foundation::Diagnostics
{
    [[noreturn]] void HandleAssertFailure(const char* expression, const char* file, int line, std::string_view message)
    {
        std::string finalMessage = "Assertion failed: ";
        finalMessage += expression;
        finalMessage += "\nFile: ";
        finalMessage += file;
        finalMessage += "\nLine: ";
        finalMessage += std::to_string(line);

        if (!message.empty())
        {
            finalMessage += "\nMessage: ";
            finalMessage += message;
        }

        throw std::runtime_error(finalMessage);
    }
}
