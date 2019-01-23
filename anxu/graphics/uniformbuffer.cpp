#include "uniformbuffer.h"

#include "device.h"

namespace anxu { namespace graphics {

UniformBuffer::UniformBuffer()
{
    XDEBUG();
}

UniformBuffer::~UniformBuffer()
{
    XDEBUG();
}

std::shared_ptr<UniformBuffer> UniformBuffer::Create(std::shared_ptr<Device> device, void* data, Info & info)
{
    XDEBUG();

    std::shared_ptr<UniformBuffer> uniformBuffer(new UniformBuffer(), UniformBuffer::Destroy);
    uniformBuffer->device = device;
    uniformBuffer->info = info;

    // Create buffer
    Buffer::Info bufferInfo(info.size, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    uniformBuffer->buffer = Buffer::Create(device, data, bufferInfo);

    if(!uniformBuffer->buffer)
    {
        XMESSAGE("Invalid buffer");
        return nullptr;
    }

    return uniformBuffer;
}

void UniformBuffer::Destroy(UniformBuffer* uniformBuffer)
{
    XDEBUG();

    assert(uniformBuffer != nullptr);

    delete uniformBuffer;
}

UniformBuffer::Info UniformBuffer::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::Buffer UniformBuffer::GetNative()
{
    XDEBUG();

    return this->buffer->GetNative();
}

std::shared_ptr<Buffer> UniformBuffer::GetBuffer()
{
    XDEBUG();

    return this->buffer;
}

bool UniformBuffer::Update(void* data, uint32_t offset, uint32_t size)
{
    return this->buffer->CopyFromData(data, offset, size);
}

} }
