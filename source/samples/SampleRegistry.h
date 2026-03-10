#pragma once

#include "samples/ISample.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace Alice::Samples
{
    class SampleRegistry
    {
    public:
        using Factory = std::function<SamplePtr()>;

        void Register(const std::string& name, Factory factory);
        SamplePtr Create(const std::string& name) const;
        std::vector<std::string> EnumerateNames() const;

    private:
        std::map<std::string, Factory> m_factories;
    };

    SampleRegistry BuildDefaultSampleRegistry();
}
