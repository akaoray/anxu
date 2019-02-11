#include "renderpass.h"

#include "device.h"

namespace anxu { namespace graphics {

RenderPass::RenderPass()
{
    XDEBUG();
}

RenderPass::~RenderPass()
{
    XDEBUG();
}

std::shared_ptr<RenderPass> RenderPass::Create(std::shared_ptr<Device> device, Info & info)
{
    XDEBUG();

    std::shared_ptr<RenderPass> renderPass(new RenderPass(), RenderPass::Destroy);
    renderPass->device = device;
    renderPass->info = info;

    std::array<vk::AttachmentDescription, 1> attachments;
    {
        attachments[0].format = info.format;
        attachments[0].samples = vk::SampleCountFlagBits::e1;
        attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
        attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
        attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        attachments[0].initialLayout = vk::ImageLayout::eUndefined;
        attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;
    }

    std::array<vk::AttachmentReference, 1> attachmentReferences;
    {
        attachmentReferences[0].attachment = 0;
        attachmentReferences[0].layout = vk::ImageLayout::eColorAttachmentOptimal;
    }

    std::array<vk::SubpassDescription, 1> subpasses;
    {
        vk::SubpassDescription& subpass = subpasses[0];
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = attachmentReferences.data();
    }

    vk::RenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.attachmentCount = (uint32_t)attachments.size();
    renderPassCreateInfo.pAttachments = attachments.data();
    renderPassCreateInfo.subpassCount = (uint32_t)subpasses.size();
    renderPassCreateInfo.pSubpasses = subpasses.data();
    renderPass->renderPass = device->GetNative().createRenderPass(renderPassCreateInfo);

    if(!renderPass->renderPass)
    {
        XMESSAGE("Invalid createRenderPass");
        return nullptr;
    }

    return renderPass;
}

void RenderPass::Destroy(RenderPass* renderPass)
{
    XDEBUG();

    assert(renderPass != nullptr);

    if(renderPass->renderPass) renderPass->device->GetNative().destroyRenderPass(renderPass->renderPass);

    delete renderPass;
}

RenderPass::Info RenderPass::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::RenderPass RenderPass::GetNative()
{
    XDEBUG();

    return this->renderPass;
}

} }
