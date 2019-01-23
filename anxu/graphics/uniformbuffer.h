#pragma once

#include "base.h"

#include "buffer.h"

namespace anxu { namespace graphics {

class UniformBuffer
{
public:
    struct Info
    {
        uint32_t size;

        Info(uint32_t size_ = 0)
            : size(size_) {}
    };

    static std::shared_ptr<UniformBuffer> Create(std::shared_ptr<Device> device, void* data, Info & info);
    static void Destroy(UniformBuffer* indexBuffer);

    Info GetInfo();
    vk::Buffer GetNative();
    std::shared_ptr<Buffer> GetBuffer();

    bool Update(void* data, uint32_t offset, uint32_t size);

private:
    UniformBuffer();
    ~UniformBuffer();

    Info info;
    std::shared_ptr<Buffer> buffer;

    std::shared_ptr<Device> device;

    DISALLOW_COPY_AND_ASSIGN(UniformBuffer);
};

} }
