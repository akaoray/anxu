#include "shader.h"

#include "device.h"

namespace anxu { namespace graphics {

Shader::Shader()
{
    XDEBUG();
}

Shader::~Shader()
{
    XDEBUG();
}

std::shared_ptr<Shader> Shader::Create(std::shared_ptr<Device> device, void* data, Info & info)
{
    XDEBUG();

    std::shared_ptr<Shader> shader(new Shader(), Shader::Destroy);
    shader->device = device;
    shader->info = info;

    shader->shaderModule = device->GetNative().createShaderModule({ {}, info.size, (const uint32_t*)data });

    if(!shader->shaderModule)
    {
        XMESSAGE("Invalid createShaderModule");
        return nullptr;
    }

    return shader;
}

void Shader::Destroy(Shader* shader)
{
    XDEBUG();

    assert(shader != nullptr);

    if(shader->shaderModule) shader->device->GetNative().destroyShaderModule(shader->shaderModule);

    delete shader;
}

Shader::Info Shader::GetInfo()
{
    XDEBUG();

    return this->info;
}

vk::ShaderModule Shader::GetNative()
{
    XDEBUG();

    return this->shaderModule;
}

} }

