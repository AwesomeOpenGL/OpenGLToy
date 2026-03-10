#include "samples/SampleRegistry.h"

#include "foundation/Diagnostics/Assert.h"

namespace Alice::Samples
{
    void SampleRegistry::Register(const std::string& name, Factory factory)
    {
        LEOCPP_ASSERT(m_factories.find(name) == m_factories.end(), "Duplicate sample name registration.");
        m_factories.emplace(name, std::move(factory));
    }

    SamplePtr SampleRegistry::Create(const std::string& name) const
    {
        const auto iterator = m_factories.find(name);
        if (iterator == m_factories.end())
        {
            return nullptr;
        }

        return iterator->second();
    }

    std::vector<std::string> SampleRegistry::EnumerateNames() const
    {
        std::vector<std::string> names;
        names.reserve(m_factories.size());

        for (const auto& pair : m_factories)
        {
            names.push_back(pair.first);
        }

        return names;
    }
}
