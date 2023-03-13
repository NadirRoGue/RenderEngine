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

#include "DeviceSelector.h"

#include <engine/vulkan/VulkanPhysicalDeviceUtils.h>

#include <iostream>

engine::WindowVulkanDevice DeviceSelector::select(const std::vector<engine::WindowVulkanPhysicalDevice> &devices)
{
    auto features = vk::PhysicalDeviceFeatures();
    features.drawIndirectFirstInstance = true;
    features.fillModeNonSolid = true;
    features.fullDrawIndexUint32 = true;
    features.imageCubeArray = true;
    features.multiDrawIndirect = true;
    // features.sampleRateShading = true;
    // features.samplerAnisotropy = true;
    features.shaderResourceMinLod = true;
    // features.tessellationShader = true;
    // features.wideLines = true;

    auto extensions = std::vector<const char *>{VK_KHR_SHADER_CLOCK_EXTENSION_NAME};

    auto queueFlags = vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eTransfer;
    auto queueCount = 2u;

    const engine::WindowVulkanPhysicalDevice *selected = nullptr;
    int selectedQueueFamily = -1;
    for (auto &device : devices)
    {
        auto &vkDevice = device.getDevice();

        if (!engine::PhysicalDeviceFeaturesChecker::satisfies(vkDevice, features))
        {
            continue;
        }

        selectedQueueFamily = engine::QueueFamilySearcher::find(vkDevice, queueFlags, queueCount);
        if (selectedQueueFamily == -1)
        {
            continue;
        }

        if (!engine::PhysicalDeviceExtensionChecker::satisfies(vkDevice, extensions))
        {
            continue;
        }

        selected = &device;
    }

    if (!selected)
    {
        throw std::runtime_error("Cannot find suitable device");
    }

    auto queuePriorities = std::vector<float>{1.f, 1.f};
    auto queueCreateInfo = vk::DeviceQueueCreateInfo(
        vk::DeviceQueueCreateFlags{0},
        static_cast<uint32_t>(selectedQueueFamily),
        queuePriorities);
    auto createInfo = vk::DeviceCreateInfo(vk::DeviceCreateFlags{0}, queueCreateInfo, {}, extensions, &features);

    auto device = engine::WindowVulkanDevice(*selected, createInfo);
    return device;
}
