#include "indexbuffer.h"

#include "device.h"

namespace anxu { namespace graphics {

IndexBuffer::IndexBuffer()
{
    XDEBUG();
}

IndexBuffer::~IndexBuffer()
{
    XDEBUG();
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(std::shared_ptr<Device> device, void* data, Info & info)
{
    XDEBUG();

    std::shared_ptr<IndexBuffer> indexBuffer(new IndexBuffer(), IndexBuffer::Destroy);
    indexBuffer->device = device;
    indexBuffer->info = info;

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
    indexBuffer->buffer = Buffer::Create(device, nullptr, bufferInfo);

    if(!indexBuffer->buffer)
    {
        XMESSAGE("Invalid buffer");
        return nullptr;
    }

    if(!indexBuffer->buffer->CopyFromBuffer(tmpBuffer))
    {
        XMESSAGE("Invalid CopyFromBuffer");
        return nullptr;
    }

    return indexBuffer;
}

void IndexBuffer::Destroy(IndexBuffer* indexBuffer)
{
    XDEBUG();

    assert(indexBuffer != nullptr);

    delete indexBuffer;
}

IndexBuffer::Info IndexBuffer::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::Buffer IndexBuffer::GetNative()
{
    XDEBUG();

    return this->buffer->GetNative();
}

std::shared_ptr<Buffer> IndexBuffer::GetBuffer()
{
    XDEBUG();

    return this->buffer;
}

} }
