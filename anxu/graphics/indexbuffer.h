#pragma once

#include "base.h"

#include "buffer.h"

namespace anxu { namespace graphics {

class IndexBuffer
{
public:
    struct Info
    {
        uint32_t size;
        vk::IndexType type;

        Info(uint32_t size_ = 0, vk::IndexType type_ = vk::IndexType::eUint32)
            : size(size_), type(type_) {}
    };

    static std::shared_ptr<IndexBuffer> Create(std::shared_ptr<Device> device, void* data, Info & info);
    static void Destroy(IndexBuffer* indexBuffer);

    Info GetInfo();
    vk::Buffer GetNative();
    std::shared_ptr<Buffer> GetBuffer();

private:
    IndexBuffer();
    ~IndexBuffer();

    Info info;
    std::shared_ptr<Buffer> buffer;

    std::shared_ptr<Device> device;

    DISALLOW_COPY_AND_ASSIGN(IndexBuffer);
};

} }
