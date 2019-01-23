#pragma once

#include "base.h"

namespace anxu { namespace graphics {

class SwapChain
{
public:
    struct Info
    {
        uint32_t width;
        uint32_t height;
        bool vsync;

        Info(uint32_t width_ = 800, uint32_t height_ = 600, bool vsync_ = true)
            : width(width_), height(height_), vsync(vsync_) {}
    };

    static std::shared_ptr<SwapChain> Create(std::shared_ptr<Surface> surface, std::shared_ptr<Device> device, Info & info);
    static void Destroy(SwapChain * swapChain);

    Info GetInfo();
    vk::Format GetFormat();
    vk::SwapchainKHR GetNative();
    std::vector<vk::ImageView> GetImageViews();

private:
    SwapChain();
    ~SwapChain();

    Info info;
    vk::Format format;
    vk::SwapchainKHR swapChain;
    std::vector<vk::Image> images;
    std::vector<vk::ImageView> imageViews;

    std::shared_ptr<Device> device;

    DISALLOW_COPY_AND_ASSIGN(SwapChain);
};

} }
