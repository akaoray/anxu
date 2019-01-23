#include "device.h"

namespace anxu { namespace graphics {

std::vector<const char*> GetDeviceRequiredExtensions()
{
    XDEBUG();

    return {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
}

Device::Device()
{
    XDEBUG();
}

Device::~Device()
{
    XDEBUG();
}

std::shared_ptr<Device> Device::Create(Info & info)
{
    XDEBUG();

    std::shared_ptr<Device> device(new Device, Device::Destroy);
    device->info = info;

    float queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo({}, static_cast<uint32_t>(info.gpu.graphicsQueueFamilyIndex), 1, &queuePriority);
    device->device = info.gpu.physicalDevice.createDevice(vk::DeviceCreateInfo({}, 1, &deviceQueueCreateInfo, 0, nullptr, static_cast<uint32_t>(info.requiredExtensions.size()), info.requiredExtensions.data()));

    if(!device->device)
    {
        XMESSAGE("Invalid createDevice");
        return nullptr;
    }

    vk::CommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.queueFamilyIndex = info.gpu.graphicsQueueFamilyIndex;
    commandPoolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    device->commandPool = device->device.createCommandPool(commandPoolCreateInfo);

    device->graphicsQueue = device->device.getQueue(info.gpu.graphicsQueueFamilyIndex, 0);
    device->presentQueue = device->device.getQueue(info.gpu.presentQueueFamilyIndex, 0);

    return device;
}

void Device::Destroy(Device * device)
{
    XDEBUG();

    assert(device != nullptr);

    if(device->device) device->device.destroy();

    delete device;
}

Device::Info Device::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::Device Device::GetNative()
{
    XDEBUG();

    return this->device;
}

vk::CommandPool Device::GetCommandPool()
{
    XDEBUG();

    return this->commandPool;
}
vk::Queue Device::GetGraphicsQueue()
{
    XDEBUG();

    return this->graphicsQueue;
}
vk::Queue Device::GetPresentQueue()
{
    XDEBUG();

    return this->presentQueue;
}

} }


