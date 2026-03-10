#pragma once

#include "samples/ISample.h"

namespace Alice::Samples::GettingStarted
{
    class SampleHelloWindow final : public ISample
    {
    public:
        bool OnInitialize(const SampleInitializationContext& context) override;
        void OnUpdate(const SampleFrameContext& context) override;
        void OnShutdown() override;
    };
}
