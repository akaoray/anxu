#pragma once

#include "base.h"

namespace anxu { namespace graphics {

class Shader
{
public:
    struct Info
    {
        vk::ShaderStageFlagBits stage;
        uint32_t size;
        std::string entry;

        Info(vk::ShaderStageFlagBits stage_ = vk::ShaderStageFlagBits::eVertex, uint32_t size_ = 0, std::string entry_ = "main")
            : stage(stage_), size(size_), entry(entry_) {}
    };

    static std::shared_ptr<Shader> Create(std::shared_ptr<Device> device, void* data, Info & info);
    static void Destroy(Shader * shader);

    Info GetInfo();
    vk::ShaderModule GetNative();
    vk::PipelineShaderStageCreateInfo GetPipelineShaderStageCreateInfo();

private:
    Shader();
    ~Shader();

    Info info;
    vk::ShaderModule shaderModule;

    std::shared_ptr<Device> device;

    DISALLOW_COPY_AND_ASSIGN(Shader);
};

} }
