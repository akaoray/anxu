#pragma once

#include "base.h"

#include "buffer.h"
#include "depthbuffer.h"
#include "device.h"
#include "gpulist.h"
#include "indexbuffer.h"
#include "instance.h"
#include "shader.h"
#include "surface.h"
#include "swapchain.h"
#include "uniformbuffer.h"
#include "vertexbuffer.h"
#include "window.h"

namespace anxu { namespace graphics {

class Graphics
{
public:
    static std::shared_ptr<Graphics> Create();
    static void Destroy(Graphics * graphics);

private:
    Graphics();
    ~Graphics();

    DISALLOW_COPY_AND_ASSIGN(Graphics);
};

} }
