#pragma once

#include "base.h"

namespace anxu { namespace graphics {

class RenderPass
{
public:
    struct Info
    {
        vk::Format format;

        Info(vk::Format format_ = vk::Format::eUndefined) :
            format(format_) {}
    };

    static std::shared_ptr<RenderPass> Create(std::shared_ptr<Device> device, Info & info);
    static void Destroy(RenderPass* renderPass);

    Info GetInfo();
    vk::RenderPass GetNative();

private:
    RenderPass();
    ~RenderPass();

    Info info;
    vk::RenderPass renderPass;

    std::shared_ptr<Device> device;

    DISALLOW_COPY_AND_ASSIGN(RenderPass);
};

} }

