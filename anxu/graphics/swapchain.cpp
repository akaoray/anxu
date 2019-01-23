#include "swapchain.h"

#include "device.h"
#include "surface.h"

namespace anxu { namespace graphics {

SwapChain::SwapChain()
{
    XDEBUG();
}

SwapChain::~SwapChain()
{
    XDEBUG();
}

std::shared_ptr<SwapChain> SwapChain::Create(std::shared_ptr<Surface> surface, std::shared_ptr<Device> device, Info & info)
{
    XDEBUG();

    std::shared_ptr<SwapChain> swapChain(new SwapChain(), SwapChain::Destroy);
    swapChain->device = device;
    swapChain->info = info;

    Gpu gpu = device->GetInfo().gpu;

    std::vector<vk::SurfaceFormatKHR> surfaceFormats = gpu.physicalDevice.getSurfaceFormatsKHR(surface->GetNative());

    if(surfaceFormats.empty())
    {
        XMESSAGE("Invalid getSurfaceFormatsKHR");
        return nullptr;
    }

    swapChain->format = (surfaceFormats[0].format == vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : surfaceFormats[0].format;

    vk::SurfaceCapabilitiesKHR surfaceCapabilities = gpu.physicalDevice.getSurfaceCapabilitiesKHR(surface->GetNative());

    VkExtent2D extent2D;
    if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
    {
        extent2D.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, info.width));
        extent2D.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, info.height));
    }
    else extent2D = surfaceCapabilities.currentExtent;

    // The FIFO present mode is guaranteed by the spec to be supported
    vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;

    vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;

    vk::CompositeAlphaFlagBitsKHR compositeAlpha = (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ?
                vk::CompositeAlphaFlagBitsKHR::ePreMultiplied : (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ?
                    vk::CompositeAlphaFlagBitsKHR::ePostMultiplied : (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ?
                        vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;

    vk::SwapchainCreateInfoKHR swapChainCreateInfo(vk::SwapchainCreateFlagsKHR(), surface->GetNative(), surfaceCapabilities.minImageCount, swapChain->format, vk::ColorSpaceKHR::eSrgbNonlinear,
                                                   extent2D, 1, vk::ImageUsageFlagBits::eColorAttachment, vk::SharingMode::eExclusive, 0, nullptr, preTransform, compositeAlpha, presentMode, true, nullptr);

    uint32_t queueFamilyIndices[2] = { static_cast<uint32_t>(gpu.graphicsQueueFamilyIndex), static_cast<uint32_t>(gpu.presentQueueFamilyIndex) };
    if (gpu.graphicsQueueFamilyIndex != gpu.presentQueueFamilyIndex)
    {
        swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    swapChain->swapChain = device->GetNative().createSwapchainKHR(swapChainCreateInfo);

    if(!swapChain->swapChain)
    {
        XMESSAGE("Invalid createSwapchainKHR");
        return nullptr;
    }

    swapChain->images = device->GetNative().getSwapchainImagesKHR(swapChain->swapChain);

    if(swapChain->images.empty())
    {
        XMESSAGE("Invalid getSwapchainImagesKHR");
        return nullptr;
    }

    swapChain->imageViews.reserve(swapChain->images.size());
    for(auto & image : swapChain->images)
    {
        vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
        vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
        vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, swapChain->format, componentMapping, subResourceRange);
        swapChain->imageViews.push_back(device->GetNative().createImageView(imageViewCreateInfo));
    }

    return swapChain;
}

void SwapChain::Destroy(SwapChain * swapChain)
{
    XDEBUG();

    assert(swapChain != nullptr);

    for(auto & imageView : swapChain->imageViews)
    {
        swapChain->device->GetNative().destroyImageView(imageView);
    }

    swapChain->device->GetNative().destroySwapchainKHR(swapChain->swapChain);

    delete swapChain;
}

SwapChain::Info SwapChain::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::Format SwapChain::GetFormat()
{
    XDEBUG();

    return this->format;
}

vk::SwapchainKHR SwapChain::GetNative()
{
    XDEBUG();

    return this->swapChain;
}

std::vector<vk::ImageView> SwapChain::GetImageViews()
{
    XDEBUG();

    return this->imageViews;
}

} }
