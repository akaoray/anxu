#include "surface.h"

#include "instance.h"
#include "window.h"

namespace anxu { namespace graphics {

Surface::Surface()
{
    XDEBUG();
}

Surface::~Surface()
{
    XDEBUG();
}

std::shared_ptr<Surface> Surface::Create(std::shared_ptr<Instance> instance, std::shared_ptr<Window> window)
{
    XDEBUG();

    std::shared_ptr<Surface> surface(new Surface, Surface::Destroy);
    surface->instance = instance;
    surface->window = window;

    VkSurfaceKHR surfaceKHR = nullptr;
    if(glfwCreateWindowSurface(instance->GetNative(), window->GetNative(), nullptr, &surfaceKHR) != VK_SUCCESS)
    {
        XMESSAGE("Invalid glfwCreateWindowSurface");
        return nullptr;
    }
    surface->surface = surfaceKHR;

    return surface;
}

void Surface::Destroy(Surface * surface)
{
    XDEBUG();

    assert(surface != nullptr);

    if(surface->surface) surface->instance->GetNative().destroySurfaceKHR(surface->surface);

    delete surface;
}

vk::SurfaceKHR Surface::GetNative()
{
    XDEBUG();

    return this->surface;
}

} }

