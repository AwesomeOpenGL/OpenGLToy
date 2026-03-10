#include "samples/BuiltInSampleRegistration.h"

#include "samples/getting_started/SampleCameraClass.h"
#include "samples/getting_started/SampleCoordinateSystems.h"
#include "samples/getting_started/SampleHelloTriangle.h"
#include "samples/getting_started/SampleHelloWindow.h"
#include "samples/getting_started/SampleShadersBasic.h"
#include "samples/getting_started/SampleTexturesBasic.h"
#include "samples/getting_started/SampleTransformations.h"

#include <memory>

namespace Alice::Samples
{
    void RegisterBuiltInSamples(SampleRegistry& registry)
    {
        registry.Register("getting_started.hello_window", []() { return std::make_unique<GettingStarted::SampleHelloWindow>(); });
        registry.Register("getting_started.hello_triangle", []() { return std::make_unique<GettingStarted::SampleHelloTriangle>(); });
        registry.Register("getting_started.shaders_basic", []() { return std::make_unique<GettingStarted::SampleShadersBasic>(); });
        registry.Register("getting_started.textures_basic", []() { return std::make_unique<GettingStarted::SampleTexturesBasic>(); });
        registry.Register("getting_started.transformations", []() { return std::make_unique<GettingStarted::SampleTransformations>(); });
        registry.Register("getting_started.coordinate_systems", []() { return std::make_unique<GettingStarted::SampleCoordinateSystems>(); });
        registry.Register("getting_started.camera_class", []() { return std::make_unique<GettingStarted::SampleCameraClass>(); });
    }

    SampleRegistry BuildDefaultSampleRegistry()
    {
        SampleRegistry registry;
        RegisterBuiltInSamples(registry);
        return registry;
    }
}
