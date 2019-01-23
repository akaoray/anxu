#include "instance.h"

namespace anxu { namespace graphics {

std::vector<const char*> GetInstanceRequiredExtensions()
{
    XDEBUG();

    std::vector<const char*> extensions;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }

    return extensions;
}

Instance::Instance()
{
    XDEBUG();
}

Instance::~Instance()
{
    XDEBUG();
}

std::shared_ptr<Instance> Instance::Create(Info & info)
{
    XDEBUG();

    std::shared_ptr<Instance> instance(new Instance, Instance::Destroy);
    instance->info = info;

    // Create instance
    vk::ApplicationInfo applicationInfo(info.applicationName.c_str(), info.applicationVersion, info.engineName.c_str(), info.engineVersion, VK_API_VERSION_1_0);
    instance->instance = vk::createInstance(vk::InstanceCreateInfo({}, &applicationInfo, 0, nullptr, info.requiredExtensions.size(), info.requiredExtensions.data()));
    if(!instance->instance)
    {
        XMESSAGE("Invalid createInstance");
        return nullptr;
    }

    return instance;
}

void Instance::Destroy(Instance * instance)
{
    XDEBUG();

    assert(instance != nullptr);

    if(instance->instance) instance->instance.destroy();

    delete instance;
}

Instance::Info Instance::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::Instance Instance::GetNative()
{
    XDEBUG();

    return this->instance;
}

} }

