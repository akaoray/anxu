#pragma once

#include "base.h"

namespace anxu { namespace graphics {

class Surface
{
public:
    static std::shared_ptr<Surface> Create(std::shared_ptr<Instance> instance, std::shared_ptr<Window> window);
    static void Destroy(Surface * surface);

    vk::SurfaceKHR GetNative();

private:
    Surface();
    ~Surface();

    vk::SurfaceKHR surface;

    std::shared_ptr<Instance> instance;
    std::shared_ptr<Window> window;

    DISALLOW_COPY_AND_ASSIGN(Surface);
};

} }
