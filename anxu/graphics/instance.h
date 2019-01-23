#pragma once

#include "base.h"

namespace anxu { namespace graphics {

std::vector<const char*> GetInstanceRequiredExtensions();

class Instance
{
public:
    struct Info
    {
        std::string applicationName;
        uint32_t applicationVersion;
        std::string engineName;
        uint32_t engineVersion;
        std::vector<const char*> requiredExtensions;

        Info(std::string applicationName_ = "applicationName", uint32_t applicationVersion_ = 1, std::string engineName_ = "engineName", uint32_t engineVersion_ = 1, std::vector<const char*> requiredExtensions_ = GetInstanceRequiredExtensions())
            : applicationName(applicationName_), applicationVersion(applicationVersion_), engineName(engineName_), engineVersion(engineVersion_), requiredExtensions(requiredExtensions_) {}
    };

    static std::shared_ptr<Instance> Create(Info & info);
    static void Destroy(Instance * instance);

    Info GetInfo();
    vk::Instance GetNative();

private:
    Instance();
    ~Instance();

    Info info;
    vk::Instance instance;

    DISALLOW_COPY_AND_ASSIGN(Instance);
};

} }
