#pragma once

#include "platform/Glfw/GlfwWindow.h"
#include "samples/SampleRegistry.h"

#include <string>

namespace Alice::App
{
    class Application
    {
    public:
        Application();

        int Run(int argc, char** argv);

    private:
        std::string ParseRequestedSampleName(int argc, char** argv, bool& shouldListSamples) const;
        int PrintUsageAndExit() const;
        int RunSelectedSample(const std::string& sampleName);

        Platform::Glfw::GlfwWindow m_window;
        Samples::SampleRegistry m_sampleRegistry;
    };
}
