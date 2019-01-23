#pragma once

#include "base.h"

namespace anxu { namespace graphics {

class Gpu
{
public:
    vk::PhysicalDevice physicalDevice;

    std::vector<vk::QueueFamilyProperties> queueFamilyProperties;
    size_t graphicsQueueFamilyIndex;
    size_t presentQueueFamilyIndex;

    vk::PhysicalDeviceProperties physicalDeviceProperties;
    vk::PhysicalDeviceFeatures physicalDeviceFeatures;
    vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

    vk::Bool32 getMemoryType(uint32_t typeBits, const vk::MemoryPropertyFlags& properties, uint32_t* typeIndex) const;
    uint32_t getMemoryType(uint32_t typeBits, const vk::MemoryPropertyFlags& properties) const;
};

class GpuList
{
public:
    static std::shared_ptr<GpuList> Create(std::shared_ptr<Instance> instance, std::shared_ptr<Surface> surface);
    static void Destroy(GpuList * gpuList);

    std::vector<Gpu> GetList();
    Gpu PickGpu();

private:
    GpuList();
    ~GpuList();

    std::vector<Gpu> gpuList;
    Gpu pickGpu;

    DISALLOW_COPY_AND_ASSIGN(GpuList);
};

} }
