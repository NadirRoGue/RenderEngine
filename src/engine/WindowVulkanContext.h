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

#pragma once

#include "vulkan/VulkanInclude.h"

namespace engine
{
class WindowVulkanPhysicalDevice
{
public:
    const vk::raii::PhysicalDevice &getDevice() const noexcept;

private:
    friend class WindowVulkanInstance;

    WindowVulkanPhysicalDevice(vk::raii::PhysicalDevice device);

    vk::raii::PhysicalDevice _device;
};

class WindowVulkanInstance
{
public:
    explicit WindowVulkanInstance(const std::vector<std::string> &extensions);

    std::vector<WindowVulkanPhysicalDevice> getAvailableDevices() const;

private:
    friend class WindowVulkanContext;

    vk::raii::Context _context;
    vk::raii::Instance _instance;
};

class WindowVulkanDevice
{
public:
    WindowVulkanDevice(const WindowVulkanPhysicalDevice &physicalDevice, const vk::DeviceCreateInfo &createInfo);

private:
    friend class WindowVulkanContext;

    vk::raii::Device _device;
};

class WindowVulkanContext
{
public:
    WindowVulkanContext(WindowVulkanInstance instance, WindowVulkanDevice device);

    const vk::raii::Instance &getInstance() const noexcept;
    const vk::raii::Device &getDevice() const noexcept;

private:
    vk::raii::Context _context;
    vk::raii::Instance _instance;
    vk::raii::Device _device;
};
}
