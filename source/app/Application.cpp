#include "app/Application.h"

#include "foundation/Diagnostics/Log.h"
#include "foundation/Time/HighResolutionClock.h"
#include "render/OpenGL/GLDebug.h"
#include "render/OpenGL/GLLoader.h"

#include <fmt/core.h>

namespace
{
    constexpr const char* GDefaultSampleName = "getting_started.hello_window";
}

namespace Alice::App
{
    Application::Application()
        : m_sampleRegistry(Samples::BuildDefaultSampleRegistry())
    {
    }

    int Application::Run(int argc, char** argv)
    {
        bool shouldListSamples = false;
        const std::string requestedSample = ParseRequestedSampleName(argc, argv, shouldListSamples);
        if (shouldListSamples)
        {
            return PrintUsageAndExit();
        }

        return RunSelectedSample(requestedSample);
    }

    std::string Application::ParseRequestedSampleName(int argc, char** argv, bool& shouldListSamples) const
    {
        shouldListSamples = false;

        for (int index = 1; index < argc; ++index)
        {
            const std::string argument = argv[index];

            if ((argument == "--sample") && index + 1 < argc)
            {
                return argv[index + 1];
            }

            if (argument == "--list-samples" || argument == "--help" || argument == "-h")
            {
                shouldListSamples = true;
                return {};
            }
        }

        return GDefaultSampleName;
    }

    int Application::PrintUsageAndExit() const
    {
        LEOCPP_LOG_INFO("Usage: Alice --sample <sample_name>");
        LEOCPP_LOG_INFO("Available samples:");
        for (const auto& sampleName : m_sampleRegistry.EnumerateNames())
        {
            LEOCPP_LOG_INFO(fmt::format("  - {}", sampleName));
        }
        return 0;
    }

    int Application::RunSelectedSample(const std::string& sampleName)
    {
        auto sample = m_sampleRegistry.Create(sampleName);
        if (sample == nullptr)
        {
            LEOCPP_LOG_ERROR(fmt::format("Unknown sample: {}", sampleName));
            return PrintUsageAndExit();
        }

        Platform::WindowDesc windowDesc;
        windowDesc.Title = fmt::format("Alice - {}", sampleName);

        if (!m_window.Create(windowDesc))
        {
            LEOCPP_LOG_ERROR("Window creation failed.");
            return 1;
        }

        if (!Render::OpenGL::InitializeOpenGLLoader(m_window.GetNativeHandle()))
        {
            return 1;
        }

    #if defined(_DEBUG)
        Render::OpenGL::EnableOpenGLDebugOutput();
    #endif

        Samples::SampleInitializationContext initializationContext{
            m_window,
            m_window.GetInputService(),
            Alice_ASSET_ROOT };

        if (!sample->OnInitialize(initializationContext))
        {
            LEOCPP_LOG_ERROR(fmt::format("Sample initialization failed: {}", sampleName));
            return 1;
        }

        Foundation::Time::HighResolutionClock clock;
        while (!m_window.ShouldClose())
        {
            m_window.GetInputService().BeginFrame();
            m_window.PollEvents();

            const double deltaSeconds = clock.Tick();
            const Samples::SampleFrameContext frameContext{
                m_window,
                m_window.GetInputService(),
                static_cast<float>(deltaSeconds),
                clock.GetElapsedSeconds() };

            sample->OnUpdate(frameContext);
            m_window.SwapBuffers();
        }

        sample->OnShutdown();
        return 0;
    }
}
