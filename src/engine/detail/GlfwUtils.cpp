/*
 * The MIT License(MIT)
 *
 * Copyright © 2022 Nadir Roman Guerrero
 *
 * This file is part of RenderEngine <https://github.com/NadirRoGue/RenderEngine>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files(the “Software”), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice and this permission notice shall be included in all copies or  substantial portions of
 *  the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "GlfwUtils.h"

#include <cassert>
#include <iostream>

namespace
{
class GlfwVulkanExtensionListImpl
{
public:
    static std::vector<std::string> compile()
    {
        assert(glfwInit());

        uint32_t glfwCount;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwCount);

        auto result = std::vector<std::string>();
        result.reserve(glfwCount);

        for (uint32_t i = 0; i < glfwCount; ++i)
        {
            result.emplace_back(glfwExtensions[i]);
        }

        return result;
    }
};

class GLFWErrorHandler
{
public:
    static void onError(int code, const char *message)
    {
        std::cerr << "[GLFW][Code " << code << "] " << message << "\n";
    }
};

class GlfwWindowFactoryImpl
{
public:
    static GLFWwindow *create(const std::string &title, uint16_t width, uint16_t height)
    {
        assert(glfwInit());

        glfwSetErrorCallback(GLFWErrorHandler::onError);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        auto w = static_cast<int>(width);
        auto h = static_cast<int>(height);
        auto window = glfwCreateWindow(w, h, title.data(), NULL, NULL);

        assert(window);

        return window;
    }
};

class GlfwSurfaceFactoryImpl
{
public:
    static vk::raii::SurfaceKHR create(const vk::raii::Instance &instance, GLFWwindow *window)
    {
        auto instanceHandle = static_cast<VkInstance>(*instance);
        auto surfaceHandle = VkSurfaceKHR();
        glfwCreateWindowSurface(instanceHandle, window, nullptr, &surfaceHandle);
        return vk::raii::SurfaceKHR(instance, surfaceHandle);
    }
};
}

namespace engine::detail
{
std::vector<std::string> GlfwVulkanExtensionList::compile()
{
    return GlfwVulkanExtensionListImpl::compile();
}

GLFWwindow *GlfwWindowFactory::create(const std::string &title, uint16_t width, uint16_t height)
{
    return GlfwWindowFactoryImpl::create(title, width, height);
}

void GlfwWindowFactory::destroy(GLFWwindow *window)
{
    glfwDestroyWindow(window);
}

vk::raii::SurfaceKHR GlfwSurfaceFactory::create(const vk::raii::Instance &instance, GLFWwindow *window)
{
    return GlfwSurfaceFactoryImpl::create(instance, window);
}
}
