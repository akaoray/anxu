#include "gpulist.h"

#include "instance.h"
#include "surface.h"

namespace anxu { namespace graphics {

vk::Bool32 Gpu::getMemoryType(uint32_t typeBits, const vk::MemoryPropertyFlags& properties, uint32_t* typeIndex) const
{
    XDEBUG();

    for (uint32_t i = 0; i < 32; i++) {
        if ((typeBits & 1) == 1) {
            if ((this->physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                *typeIndex = i;
                return VK_TRUE;
            }
        }
        typeBits >>= 1;
    }
    return VK_FALSE;
}

uint32_t Gpu::getMemoryType(uint32_t typeBits, const vk::MemoryPropertyFlags& properties) const
{
    XDEBUG();

    uint32_t result = 0;
    if (VK_FALSE == getMemoryType(typeBits, properties, &result)) {
        XMESSAGE("Unable to find memory type " + vk::to_string(properties));
    }
    return result;
}

GpuList::GpuList()
{
    XDEBUG();
}

GpuList::~GpuList()
{
    XDEBUG();
}

std::shared_ptr<GpuList> GpuList::Create(std::shared_ptr<Instance> instance, std::shared_ptr<Surface> surface)
{
    XDEBUG();

    std::shared_ptr<GpuList> gpuList(new GpuList, GpuList::Destroy);

    // Get physical devices
    std::vector<vk::PhysicalDevice> physicalDevices = instance->GetNative().enumeratePhysicalDevices();

    if(physicalDevices.empty())
    {
        XMESSAGE("Invalid enumeratePhysicalDevices");
        return nullptr;
    }

    // Get gpus info and pick gpu
    for(auto & physicalDevice : physicalDevices)
    {
        Gpu gpu = {};
        gpu.physicalDevice = physicalDevice;
        gpu.queueFamilyProperties = gpu.physicalDevice.getQueueFamilyProperties();

        // Get graphicsQueueFamilyIndex
        gpu.graphicsQueueFamilyIndex = std::distance(gpu.queueFamilyProperties.begin(),
                                                     std::find_if(gpu.queueFamilyProperties.begin(),
                                                                  gpu.queueFamilyProperties.end(),
                                                                  [](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));

        // Get presentQueueFamilyIndex
        gpu.presentQueueFamilyIndex = gpu.physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(gpu.graphicsQueueFamilyIndex), surface->GetNative()) ? gpu.graphicsQueueFamilyIndex : gpu.queueFamilyProperties.size();
        if (gpu.presentQueueFamilyIndex == gpu.queueFamilyProperties.size())
        {
            // the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both graphics and present
            for (size_t i = 0; i < gpu.queueFamilyProperties.size(); i++)
            {
                if ((gpu.queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) && gpu.physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface->GetNative()))
                {
                    gpu.graphicsQueueFamilyIndex = i;
                    gpu.presentQueueFamilyIndex = i;
                    break;
                }
            }
            if (gpu.presentQueueFamilyIndex == gpu.queueFamilyProperties.size())
            {
                // there's nothing like a single family index that supports both graphics and present -> look for an other family index that supports present
                for (size_t i = 0; i < gpu.queueFamilyProperties.size(); i++)
                {
                    if (gpu.physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface->GetNative()))
                    {
                        gpu.presentQueueFamilyIndex = i;
                        break;
                    }
                }
            }
        }
        if ((gpu.graphicsQueueFamilyIndex == gpu.queueFamilyProperties.size()) || (gpu.presentQueueFamilyIndex == gpu.queueFamilyProperties.size()))
        {
            // Ignore not supported device
            continue;
        }

        gpu.physicalDeviceProperties = gpu.physicalDevice.getProperties();
        gpu.physicalDeviceFeatures = gpu.physicalDevice.getFeatures();
        gpu.physicalDeviceMemoryProperties = gpu.physicalDevice.getMemoryProperties();

        if((gpu.physicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) && !gpuList->pickGpu.physicalDevice)
        {
            gpuList->pickGpu = gpu;
        }

        gpuList->gpuList.push_back(gpu);
    }

    if(gpuList->gpuList.empty())
    {
        XMESSAGE("Invalid find gpus");
        return nullptr;
    }

    if(!gpuList->pickGpu.physicalDevice)
    {
        XMESSAGE("Invalid pick eDiscreteGpu, will be picked first valid");
        gpuList->pickGpu = gpuList->gpuList[0];
    }

    XMESSAGE(gpuList->pickGpu.physicalDeviceProperties.deviceName<<" was set as default device");

    return gpuList;
}

void GpuList::Destroy(GpuList * gpuList)
{
    XDEBUG();

    assert(gpuList != nullptr);

    delete gpuList;
}

std::vector<Gpu> GpuList::GetList()
{
    XDEBUG();

    return this->gpuList;
}

Gpu GpuList::PickGpu()
{
    XDEBUG();

    return this->pickGpu;
}

} }
