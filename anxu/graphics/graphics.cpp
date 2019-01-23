#include "graphics.h"

namespace anxu { namespace graphics {

Graphics::Graphics()
{
    XDEBUG();
}

Graphics::~Graphics()
{
    XDEBUG();
}

std::shared_ptr<Graphics> Graphics::Create()
{
    XDEBUG();

    std::shared_ptr<Graphics> graphics(new Graphics(), Graphics::Destroy);

    if(glfwInit() != GLFW_TRUE)
    {
        XMESSAGE("Invalid glfwInit");
        return nullptr;
    }

    if (glfwVulkanSupported() != GLFW_TRUE)
    {
        XMESSAGE("Invalid glfwVulkanSupported");
        return nullptr;
    }

    return graphics;
}

void Graphics::Destroy(Graphics * graphics)
{
    XDEBUG();

    assert(graphics != nullptr);

    delete graphics;

    glfwTerminate();
}

} }

