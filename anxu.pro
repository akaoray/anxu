TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl -lvulkan

SOURCES += main.cpp \
    anxu/graphics/graphics.cpp \
    anxu/graphics/instance.cpp \
    anxu/graphics/window.cpp \
    anxu/graphics/gpulist.cpp \
    anxu/graphics/surface.cpp \
    anxu/graphics/device.cpp \
    anxu/graphics/swapchain.cpp \
    anxu/graphics/vertexbuffer.cpp \
    anxu/graphics/buffer.cpp \
    anxu/graphics/indexbuffer.cpp \
    anxu/graphics/uniformbuffer.cpp \
    anxu/graphics/shader.cpp \
    anxu/graphics/depthbuffer.cpp \
    anxu/graphics/renderpass.cpp \
    anxu/graphics/pipeline.cpp

HEADERS += \
    anxu/graphics/graphics.h \
    anxu/graphics/base.h \
    anxu/base.h \
    anxu/graphics/instance.h \
    anxu/graphics/window.h \
    anxu/graphics/gpulist.h \
    anxu/graphics/surface.h \
    anxu/graphics/device.h \
    anxu/graphics/swapchain.h \
    anxu/graphics/vertexbuffer.h \
    anxu/graphics/buffer.h \
    anxu/graphics/indexbuffer.h \
    anxu/graphics/uniformbuffer.h \
    anxu/graphics/shader.h \
    anxu/graphics/depthbuffer.h \
    anxu/graphics/renderpass.h \
    anxu/graphics/pipeline.h

