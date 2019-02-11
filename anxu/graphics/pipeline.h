#pragma once

#include "base.h"

namespace anxu { namespace graphics {

class Pipeline
{
public:
    struct Info
    {
        std::vector<Shader> shaders;
        std::vector<vk::Viewport> viewports;
        std::vector<vk::Rect2D> scissors;
        Info(std::vector<Shader> shaders_ = {}, std::vector<vk::Viewport> viewports_ = {}, std::vector<vk::Rect2D> scissors_ = {}) :
            shaders(shaders_), viewports(viewports_), scissors(scissors_) {}
    };

    static std::shared_ptr<Pipeline> Create(std::shared_ptr<Device> device, Info & info);
    static void Destroy(Pipeline * pipeline);

    Info GetInfo();
    vk::Pipeline GetNative();

private:
    Pipeline();
    ~Pipeline();

    Info info;
    vk::Pipeline pipeline;

    std::shared_ptr<Device> device;

    DISALLOW_COPY_AND_ASSIGN(Pipeline);
};

} }
