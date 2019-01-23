#include "buffer.h"

#include "device.h"

namespace anxu { namespace graphics {

Buffer::Buffer()
{
    XDEBUG();
}

Buffer::~Buffer()
{
    XDEBUG();
}

std::shared_ptr<Buffer> Buffer::Create(std::shared_ptr<Device> device, void* data, Info & info)
{
    XDEBUG();

    std::shared_ptr<Buffer> buffer(new Buffer(), Buffer::Destroy);
    buffer->device = device;
    buffer->info = info;

    // Create buffer
    vk::BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.size = info.size;
    bufferCreateInfo.usage = info.usage;
    buffer->buffer = device->GetNative().createBuffer(bufferCreateInfo);

    if(!buffer->buffer)
    {
        XMESSAGE("Invalid createBuffer");
        return nullptr;
    }

    // Create buffer memory
    vk::MemoryRequirements memoryRequirements;
    vk::MemoryAllocateInfo memoryAllocateInfo;
    memoryRequirements = device->GetNative().getBufferMemoryRequirements(buffer->buffer);
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = device->GetInfo().gpu.getMemoryType(memoryRequirements.memoryTypeBits, info.properties);
    buffer->bufferMemory = device->GetNative().allocateMemory(memoryAllocateInfo);

    if(!buffer->bufferMemory)
    {
        XMESSAGE("Invalid allocateMemory");
        return nullptr;
    }

    if(data != nullptr && (info.properties & vk::MemoryPropertyFlagBits::eHostVisible))
    {
        // Copy data to tmp buffer memory
        void* mapMem = device->GetNative().mapMemory(buffer->bufferMemory, 0, info.size, vk::MemoryMapFlags());
        memcpy(mapMem, data, info.size);
        device->GetNative().unmapMemory(buffer->bufferMemory);
    }

    // Bind memory to buffer
    device->GetNative().bindBufferMemory(buffer->buffer, buffer->bufferMemory, 0);

    return buffer;
}

void Buffer::Destroy(Buffer* buffer)
{
    XDEBUG();

    assert(buffer != nullptr);

    if(buffer->bufferMemory) buffer->device->GetNative().freeMemory(buffer->bufferMemory);
    if(buffer->buffer) buffer->device->GetNative().destroyBuffer(buffer->buffer);

    delete buffer;
}

Buffer::Info Buffer::GetInfo()
{
   XDEBUG();

   return this->info;
}

vk::Buffer Buffer::GetNative()
{
    XDEBUG();

    return this->buffer;
}

void* Buffer::Map(uint32_t offset, uint32_t size)
{
    XDEBUG();

    if(!(this->info.properties & vk::MemoryPropertyFlagBits::eHostVisible))
    {
        XMESSAGE("Invalid buffer mode");
        return nullptr;
    }

    if(offset + size > this->info.size)
    {
        XMESSAGE("Invalid buffer offset or size");
        return nullptr;
    }

    return this->device->GetNative().mapMemory(this->bufferMemory, offset, size, vk::MemoryMapFlags());
}

void Buffer::Unmap()
{
    XDEBUG();

    if(!(this->info.properties & vk::MemoryPropertyFlagBits::eHostVisible))
    {
        XMESSAGE("Invalid buffer mode");
        return;
    }

    this->device->GetNative().unmapMemory(this->bufferMemory);
}

bool Buffer::CopyFromData(void* data, uint32_t offset, uint32_t size)
{
    XDEBUG();

    if(!(this->info.properties & vk::MemoryPropertyFlagBits::eHostVisible))
    {
        XMESSAGE("Invalid buffer mode");
        return false;
    }

    if(data == nullptr)
    {
        XMESSAGE("Invalid buffer data");
        return false;
    }

    if(offset + size > this->info.size)
    {
        XMESSAGE("Invalid buffer offset or size");
        return false;
    }

    void* mapMem = this->device->GetNative().mapMemory(this->bufferMemory, offset, size, vk::MemoryMapFlags());
    memcpy(mapMem, data, size);
    this->device->GetNative().unmapMemory(this->bufferMemory);

    return true;
}

bool Buffer::CopyFromBuffer(std::shared_ptr<Buffer> buffer)
{
    XDEBUG();

    if(!(this->info.usage & vk::BufferUsageFlagBits::eTransferDst))
    {
        XMESSAGE("Invalid buffer DST type");
        return false;
    }

    if(!(buffer->GetInfo().usage & vk::BufferUsageFlagBits::eTransferSrc))
    {
        XMESSAGE("Invalid buffer SRC type");
        return false;
    }

    if(this->info.size != buffer->GetInfo().size)
    {
        XMESSAGE("Invalid buffer sizes not equal");
        return false;
    }

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.commandPool = device->GetCommandPool();
    commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;
    commandBufferAllocateInfo.commandBufferCount = 1;
    vk::CommandBuffer commandBuffer = device->GetNative().allocateCommandBuffers(commandBufferAllocateInfo)[0];

    if(!commandBuffer)
    {
        XMESSAGE("Invalid allocateCommandBuffers");
        return false;
    }

    vk::CommandBufferBeginInfo commandBufferBeginInfo;
    vk::BufferCopy bufferCopy;

    commandBuffer.begin(commandBufferBeginInfo);
    bufferCopy.size = this->info.size;
    commandBuffer.copyBuffer(buffer->GetNative(), this->buffer, bufferCopy);

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vk::Fence fence;
    this->device->GetGraphicsQueue().submit(submitInfo, fence);
    this->device->GetGraphicsQueue().waitIdle();

    this->device->GetNative().freeCommandBuffers(device->GetCommandPool(), commandBuffer);

    return true;
}

} }
