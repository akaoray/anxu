#include "vertexbuffer.h"

#include "device.h"

namespace anxu { namespace graphics {

VertexBuffer::VertexBuffer()
{
    XDEBUG();
}

VertexBuffer::~VertexBuffer()
{
    XDEBUG();
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(std::shared_ptr<Device> device, void* data, Info & info)
{
    XDEBUG();

    std::shared_ptr<VertexBuffer> vertexBuffer(new VertexBuffer(), VertexBuffer::Destroy);
    vertexBuffer->device = device;
    vertexBuffer->info = info;

    // Create tmp buffer
    Buffer::Info tmpBufferInfo(info.size, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible);
    std::shared_ptr<Buffer> tmpBuffer = Buffer::Create(device, data, tmpBufferInfo);

    if(!tmpBuffer)
    {
        XMESSAGE("Invalid tmpBuffer");
        return nullptr;
    }

    // Create dst buffer
    Buffer::Info bufferInfo(info.size, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal);
    vertexBuffer->buffer = Buffer::Create(device, nullptr, bufferInfo);

    if(!vertexBuffer->buffer)
    {
        XMESSAGE("Invalid buffer");
        return nullptr;
    }

    if(!vertexBuffer->buffer->CopyFromBuffer(tmpBuffer))
    {
        XMESSAGE("Invalid CopyFromBuffer");
        return nullptr;
    }

    return vertexBuffer;
}

void VertexBuffer::Destroy(VertexBuffer* vertexBuffer)
{
    XDEBUG();

    assert(vertexBuffer != nullptr);

    delete vertexBuffer;
}

VertexBuffer::Info VertexBuffer::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::Buffer VertexBuffer::GetNative()
{
    XDEBUG();

    return this->buffer->GetNative();
}

std::shared_ptr<Buffer> VertexBuffer::GetBuffer()
{
    XDEBUG();

    return this->buffer;
}

} }
