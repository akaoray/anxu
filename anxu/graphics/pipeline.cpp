#include "pipeline.h"

#include "device.h"
#include "shader.h"

namespace anxu { namespace graphics {

Pipeline::Pipeline()
{
    XDEBUG();
}

Pipeline::~Pipeline()
{
    XDEBUG();
}

std::shared_ptr<Pipeline> Pipeline::Create(std::shared_ptr<Device> device, Info & info)
{
    XDEBUG();

    std::shared_ptr<Pipeline> pipeline(new Pipeline(), Pipeline::Destroy);
    pipeline->device = device;
    pipeline->info = info;

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
    shaderStages.resize(info.shaders.size());
    for(uint32_t i = 0; i < shaderStages.size(); i++)
        shaderStages[i] = info.shaders[i]->GetPipelineShaderStageCreateInfo();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    vk::PipelineViewportStateCreateInfo viewportState = {};
    viewportState.viewportCount = info.viewports.size();
    viewportState.pViewports = info.viewports.data();
    viewportState.scissorCount = info.scissors.size();
    viewportState.pScissors = info.scissors.data();

    vk::PipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;

    vk::PipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    pipeline->pipeline = device->GetNative().createPipelineLayout(pipelineLayoutInfo);

    if(!pipeline->pipeline)
    {
        XMESSAGE("Invalid createPipelineLayout");
        return nullptr;
    }

    return pipeline;
}

void Pipeline::Destroy(Pipeline * pipeline)
{
    XDEBUG();

    assert(shader != nullptr);

    if(shader->shaderModule) shader->device->GetNative().destroyShaderModule(shader->shaderModule);

    delete shader;
}

Pipeline::Info Pipeline::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::Pipeline Pipeline::GetNative()
{
    XDEBUG();

    return this->pipeline;
}

} }

