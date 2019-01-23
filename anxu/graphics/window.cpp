#include "window.h"

namespace anxu { namespace graphics {

Window::Window()
{
    XDEBUG();
}

Window::~Window()
{
    XDEBUG();
}

std::shared_ptr<Window> Window::Create(Info & info)
{
    XDEBUG();

    std::shared_ptr<Window> window(new Window(), Window::Destroy);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window->info = info;
    window->window = glfwCreateWindow(info.width, info.height, info.title.c_str(), nullptr, nullptr);

    if(window->window == nullptr)
    {
        XMESSAGE("Invalid glfwCreateWindow");
        return nullptr;
    }

    return window;
}

void Window::Destroy(Window * window)
{
    XDEBUG();

    assert(window != nullptr);

    if(window->window) glfwDestroyWindow(window->window);
    delete window;
}

Window::Info Window::GetInfo()
{
    XDEBUG();

    return this->info;
}

GLFWwindow* Window::GetNative()
{
    XDEBUG();

    return this->window;
}

} }

