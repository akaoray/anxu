#pragma once

#include "base.h"

namespace anxu { namespace graphics {

class Buffer
{
public:
    struct Info
    {
        uint32_t size;
        vk::BufferUsageFlags usage;
        vk::MemoryPropertyFlags properties;

        Info(uint32_t size_ = 0, vk::BufferUsageFlags usage_ = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlags properties_ = vk::MemoryPropertyFlagBits::eDeviceLocal)
            : size(size_), usage(usage_), properties(properties_) {}
    };

    static std::shared_ptr<Buffer> Create(std::shared_ptr<Device> device, void* data, Info & info);
    static void Destroy(Buffer* buffer);

    Info GetInfo();
    vk::Buffer GetNative();

    void* Map(uint32_t offset, uint32_t size);
    void Unmap();

    bool CopyFromData(void* data, uint32_t offset, uint32_t size);
    bool CopyFromBuffer(std::shared_ptr<Buffer> buffer);

private:
    Buffer();
    ~Buffer();

    Info info;
    vk::Buffer buffer;
    vk::DeviceMemory bufferMemory;

    std::shared_ptr<Device> device;

    DISALLOW_COPY_AND_ASSIGN(Buffer);
};

} }
