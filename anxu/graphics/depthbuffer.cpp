#include "depthbuffer.h"

#include "device.h"

namespace anxu { namespace graphics {

DepthBuffer::DepthBuffer()
{
    XDEBUG();
}

DepthBuffer::~DepthBuffer()
{
    XDEBUG();
}

std::shared_ptr<DepthBuffer> DepthBuffer::Create(std::shared_ptr<Device> device, Info & info)
{
    XDEBUG();

    std::shared_ptr<DepthBuffer> depthBuffer(new DepthBuffer(), DepthBuffer::Destroy);
    depthBuffer->device = device;
    depthBuffer->info = info;

    vk::FormatProperties formatProperties = device->GetInfo().gpu.physicalDevice.getFormatProperties(info.format);

    vk::ImageTiling tiling;
    if(formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
    {
        tiling = vk::ImageTiling::eLinear;
    }
    else if(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
    {
        tiling = vk::ImageTiling::eOptimal;
    }
    else
    {
        XMESSAGE("Invalid format! DepthStencilAttachment is not supported current depth format");
        return nullptr;
    }

    vk::ImageCreateInfo imageCreateInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, info.format, vk::Extent3D(info.width, info.height, 1), 1, 1, vk::SampleCountFlagBits::e1, tiling, vk::ImageUsageFlagBits::eDepthStencilAttachment);
    depthBuffer->image = device->GetNative().createImage(imageCreateInfo);

    if(!depthBuffer->image)
    {
        XMESSAGE("Invalid createImage");
        return nullptr;
    }

    vk::PhysicalDeviceMemoryProperties memoryProperties = device->GetInfo().gpu.physicalDevice.getMemoryProperties();
    vk::MemoryRequirements memoryRequirements = device->GetNative().getImageMemoryRequirements(depthBuffer->image);
    uint32_t typeBits = memoryRequirements.memoryTypeBits;
    uint32_t typeIndex = uint32_t(~0);
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
      if ((typeBits & 1) && ((memoryProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal) == vk::MemoryPropertyFlagBits::eDeviceLocal))
      {
        typeIndex = i;
        break;
      }
      typeBits >>= 1;
    }
    assert(typeIndex != ~0);
    depthBuffer->imageMemory = device->GetNative().allocateMemory(vk::MemoryAllocateInfo(memoryRequirements.size, typeIndex));

    if(!depthBuffer->imageMemory)
    {
        XMESSAGE("Invalid allocateMemory");
        return nullptr;
    }

    device->GetNative().bindImageMemory(depthBuffer->image, depthBuffer->imageMemory, 0);

    vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
    vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1);
    depthBuffer->imageView = device->GetNative().createImageView(vk::ImageViewCreateInfo(vk::ImageViewCreateFlags(), depthBuffer->image, vk::ImageViewType::e2D, info.format, componentMapping, subResourceRange));

    if(!depthBuffer->imageView)
    {
        XMESSAGE("Invalid createImageView");
        return nullptr;
    }

    return depthBuffer;
}

void DepthBuffer::Destroy(DepthBuffer * depthBuffer)
{
    XDEBUG();

    assert(depthBuffer != nullptr);

    if(depthBuffer->imageView) depthBuffer->device->GetNative().destroyImageView(depthBuffer->imageView);

    if(depthBuffer->imageMemory) depthBuffer->device->GetNative().freeMemory(depthBuffer->imageMemory);

    if(depthBuffer->image) depthBuffer->device->GetNative().destroyImage(depthBuffer->image);

    delete depthBuffer;
}

DepthBuffer::Info DepthBuffer::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::Image DepthBuffer::GetImage()
{
    XDEBUG();

    return this->image;
}

vk::DeviceMemory DepthBuffer::GetImageMemory()
{
    XDEBUG();

    return this->imageMemory;
}

vk::ImageView DepthBuffer::GetImageView()
{
    XDEBUG();

    return this->imageView;
}

} }


