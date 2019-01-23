#pragma once

#include "base.h"

namespace anxu { namespace graphics {

class Window
{
public:
    struct Info
    {
        std::string title;
        uint32_t width;
        uint32_t height;

        Info(std::string title_ = "title", uint32_t width_ = 800, uint32_t height_ = 600)
            : title(title_), width(width_), height(height_) {}
    };

    static std::shared_ptr<Window> Create(Info & info);
    static void Destroy(Window * window);

    Info GetInfo();
    GLFWwindow* GetNative();

private:
    Window();
    ~Window();

    Info info;
    GLFWwindow* window;

    DISALLOW_COPY_AND_ASSIGN(Window);
};

} }
