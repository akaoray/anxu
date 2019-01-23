#pragma once

#include "base.h"

#include "gpulist.h"

namespace anxu { namespace graphics {

class DepthBuffer
{
public:
    struct Info
    {
        vk::Format format;
        uint32_t width;
        uint32_t height;

        Info(vk::Format format_ = vk::Format::eD16Unorm, uint32_t width_ = 0, uint32_t height_ = 0)
            : format(format_),  width(width_), height(height_) {}
    };

    static std::shared_ptr<DepthBuffer> Create(std::shared_ptr<Device> device, Info & info);
    static void Destroy(DepthBuffer * depthBuffer);

    Info GetInfo();
    vk::Image GetImage();
    vk::DeviceMemory GetImageMemory();
    vk::ImageView GetImageView();

private:
    DepthBuffer();
    ~DepthBuffer();

    Info info;

    vk::Image image;
    vk::DeviceMemory imageMemory;
    vk::ImageView imageView;

    std::shared_ptr<Device> device;

    DISALLOW_COPY_AND_ASSIGN(DepthBuffer);
};

} }
