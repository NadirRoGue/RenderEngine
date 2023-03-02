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

#include "WindowVulkanContext.h"

#include "vulkan/VulkanInstanceUtils.h"

#include "detail/GlfwUtils.h"

#include <GLFW/glfw3.h>

#include <cassert>

namespace
{
class ExtensionListBuilder
{
public:
    static std::vector<std::string> withSurfaceExtensions(const std::vector<std::string> &input)
    {
        auto glfwExtensions = engine::detail::GlfwVulkanExtensionList::compile();

        auto result = std::vector<std::string>();
        result.reserve(input.size() + glfwExtensions.size());
        result.insert(result.end(), input.begin(), input.end());
        result.insert(result.end(), glfwExtensions.begin(), glfwExtensions.end());

        return result;
    }
};

class PhysicalDeviceValidator
{
public:
    static bool isValid(const vk::raii::PhysicalDevice &device, const vk::raii::SurfaceKHR &surface)
    {
        auto formats = device.getSurfaceFormatsKHR(*surface);
        auto presentModes = device.getSurfacePresentModesKHR(*surface);
        return !formats.empty() && !presentModes.empty();
    }
};

class WindowEnabledDeviceList
{
public:
    static vk::raii::PhysicalDevices compile(const vk::raii::Instance &instance)
    {
        auto tempWindow = engine::detail::GlfwWindowFactory::create("", 1, 1);
        auto tempSurface = engine::detail::GlfwSurfaceFactory::create(instance, tempWindow);

        auto allDevices = instance.enumeratePhysicalDevices();
        assert(!allDevices.empty());

        auto validDevices = vk::raii::PhysicalDevices(instance);
        validDevices.reserve(allDevices.size());

        for (auto &device : allDevices)
        {
            if (!PhysicalDeviceValidator::isValid(device, tempSurface))
            {
                continue;
            }

            validDevices.push_back(std::move(device));
        }

        return validDevices;
    }
};
}

namespace engine
{
const vk::raii::PhysicalDevice &WindowVulkanPhysicalDevice::getDevice() const noexcept
{
    return _device;
}

WindowVulkanPhysicalDevice::WindowVulkanPhysicalDevice(vk::raii::PhysicalDevice device)
    : _device(std::move(device))
{
}

WindowVulkanInstance::WindowVulkanInstance(const std::vector<std::string> &extensions)
    : _instance(VulkanInstanceUtils::createInstance(_context, ExtensionListBuilder::withSurfaceExtensions(extensions)))
{
}

std::vector<WindowVulkanPhysicalDevice> WindowVulkanInstance::getAvailableDevices() const
{
    auto validDevices = WindowEnabledDeviceList::compile(_instance);

    auto windowDevices = std::vector<WindowVulkanPhysicalDevice>();
    windowDevices.reserve(validDevices.size());

    for (auto &device : validDevices)
    {
        windowDevices.push_back(WindowVulkanPhysicalDevice(std::move(device)));
    }

    return windowDevices;
}

WindowVulkanDevice::WindowVulkanDevice(
    const WindowVulkanPhysicalDevice &physicalDevice,
    const vk::DeviceCreateInfo &createInfo)
    : _device(physicalDevice.getDevice().createDevice(createInfo))
{
}

WindowVulkanContext::WindowVulkanContext(WindowVulkanInstance instance, WindowVulkanDevice device)
    : _context(std::move(instance._context))
    , _instance(std::move(instance._instance))
    , _device(std::move(device._device))
{
}

const vk::raii::Instance &WindowVulkanContext::getInstance() const noexcept
{
    return _instance;
}

const vk::raii::Device &WindowVulkanContext::getDevice() const noexcept
{
    return _device;
}
}
