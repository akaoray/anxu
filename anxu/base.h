#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <string>
#include <limits>
#include <fstream>
#include <cstring>
#include <assert.h>
#include <algorithm>
#include <unistd.h>

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#define XMESSAGE(message) std::cout<<message<<std::endl

#define DISALLOW_COPY_AND_ASSIGN(class_name) \
    class_name(const class_name& root) = delete; \
    class_name& operator=(const class_name&) = delete

#define ANXU_DEBUG

#if defined ANXU_DEBUG
#define XDEBUG() std::cout<<__PRETTY_FUNCTION__<<std::endl
#else
#define XDEBUG()
#endif

#define ANXU_NAME "anxu"
#define ANXU_VERSION 1
