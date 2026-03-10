#pragma once

#include "platform/IInputService.h"
#include "platform/IWindow.h"

#include <memory>
#include <string>

namespace Alice::Samples
{
    struct SampleInitializationContext
    {
        Platform::IWindow& Window;
        Platform::IInputService& Input;
        std::string AssetRoot;
    };

    struct SampleFrameContext
    {
        Platform::IWindow& Window;
        Platform::IInputService& Input;
        float DeltaSeconds = 0.0f;
        double ElapsedSeconds = 0.0;
    };

    class ISample
    {
    public:
        virtual ~ISample() = default;

        virtual bool OnInitialize(const SampleInitializationContext& context) = 0;
        virtual void OnUpdate(const SampleFrameContext& context) = 0;
        virtual void OnShutdown() = 0;
    };

    using SamplePtr = std::unique_ptr<ISample>;
}
