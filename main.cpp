#include "anxu/graphics/graphics.h"

using namespace anxu::graphics;

int main()
{
    //
    auto graphics = Graphics::Create();

    //
    Instance::Info instanceInfo;
    instanceInfo.applicationName = "Application";
    instanceInfo.applicationVersion = 1;
    instanceInfo.engineName = ANXU_NAME;
    instanceInfo.engineVersion = ANXU_VERSION;
    auto instance = Instance::Create(instanceInfo);

    //
    Window::Info windowInfo;
    windowInfo.title = "Application";
    windowInfo.width = 800;
    windowInfo.height = 600;
    auto window = Window::Create(windowInfo);

    //
    auto surface = Surface::Create(instance, window);

    //
    auto gpuList = GpuList::Create(instance, surface);

    //
    Device::Info deviceInfo;
    deviceInfo.gpu = gpuList->PickGpu();
    deviceInfo.requiredExtensions = GetDeviceRequiredExtensions();
    auto device = Device::Create(deviceInfo);

    //
    SwapChain::Info swapChainInfo;
    swapChainInfo.width = 800;
    swapChainInfo.height = 600;
    swapChainInfo.vsync = false;
    auto swapChain = SwapChain::Create(surface, device, swapChainInfo);

    //
    Buffer::Info bufferInfo;
    bufferInfo.size = 256;
    bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst;
    bufferInfo.properties = vk::MemoryPropertyFlagBits::eDeviceLocal;
    auto buffer = Buffer::Create(device, nullptr, bufferInfo);

    //
    struct Vertex {
        float pos[3];
        float col[3];
    };

    std::vector<Vertex> vertexBufferData = { { { 1.0f, 1.0f, 0.0f },
                                 { 1.0f,
                                   0.0f,
                                   0.0f } },
                               { { -1.0f, 1.0f, 0.0f },
                                 { 0.0f,
                                   1.0f,
                                   0.0f } },
                               { { 0.0f, -1.0f, 0.0f },
                                 { 0.0f,
                                   0.0f,
                                   1.0f } } };

    uint32_t vertexBufferSize = (uint32_t)(vertexBufferData.size() * sizeof(Vertex));

    VertexBuffer::Info vertexBufferInfo(vertexBufferSize, sizeof(Vertex), {});
    auto vertexBuffer = VertexBuffer::Create(device, vertexBufferData.data(), vertexBufferInfo);

    //
    std::vector<uint32_t> indexBufferData = {0, 1, 2};
    uint32_t indexBufferSize = (uint32_t)(indexBufferData.size() * sizeof(uint32_t));

    IndexBuffer::Info indexBufferInfo(indexBufferSize, vk::IndexType::eUint32);
    auto indexBuffer = IndexBuffer::Create(device, indexBufferData.data(), indexBufferInfo);

    //
    struct uboVS{
        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
    };

    uboVS uniformBufferObjectVS;

    UniformBuffer::Info uniformBufferInfo(sizeof(uboVS));
    auto uniformBuffer = UniformBuffer::Create(device, &uniformBufferObjectVS, uniformBufferInfo);

    //
    DepthBuffer::Info depthBufferInfo;
    depthBufferInfo.format = vk::Format::eD16Unorm;
    depthBufferInfo.width = 800;
    depthBufferInfo.height = 600;
    auto depthBuffer = DepthBuffer::Create(device, depthBufferInfo);

    sleep(1);

    XMESSAGE("================================================================");

    return 0;
}

