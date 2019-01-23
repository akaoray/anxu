#pragma once

#include "base.h"

#include "buffer.h"

namespace anxu { namespace graphics {

class VertexBuffer
{
public:
    enum class LayoutType
    {
        POSITION,
        COLOR,
    };

    struct Layout
    {
        LayoutType layoutType;
        uint32_t size;
    };

    struct Info
    {
        uint32_t size;
        uint32_t stride;
        std::vector<Layout> layouts;

        Info(uint32_t size_ = 0, uint32_t stride_ = 0, std::vector<Layout> layouts_ = {})
            : size(size_), stride(stride_), layouts(layouts_) {}
    };

    static std::shared_ptr<VertexBuffer> Create(std::shared_ptr<Device> device, void* data, Info & info);
    static void Destroy(VertexBuffer* vertexBuffer);

    Info GetInfo();
    vk::Buffer GetNative();
    std::shared_ptr<Buffer> GetBuffer();

private:
    VertexBuffer();
    ~VertexBuffer();

    Info info;
    std::shared_ptr<Buffer> buffer;

    std::shared_ptr<Device> device;

    DISALLOW_COPY_AND_ASSIGN(VertexBuffer);
};

} }
