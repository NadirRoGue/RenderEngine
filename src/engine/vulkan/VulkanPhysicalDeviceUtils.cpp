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

#include "VulkanPhysicalDeviceUtils.h"

#include <algorithm>

namespace
{
class PhysicalDeviceFeaturesCheckerImpl
{
public:
    static bool satisfies(const vk::raii::PhysicalDevice &device, const vk::PhysicalDeviceFeatures &features)
    {
        constexpr auto size = sizeof(vk::PhysicalDeviceFeatures) / sizeof(VkBool32);

        auto target = reinterpret_cast<const VkBool32 *>(&features);

        auto deviceFeatures = device.getFeatures();
        auto pool = reinterpret_cast<const VkBool32 *>(&deviceFeatures);

        for (size_t i = 0; i < size; ++i)
        {
            if (target[i] && !pool[i])
            {
                return false;
            }
        }

        return true;
    }
};

class QueueFamilySearcherImpl
{
public:
    static int find(const vk::raii::PhysicalDevice &device, vk::QueueFlags flags, uint32_t queueCount)
    {
        auto families = device.getQueueFamilyProperties();

        for (size_t i = 0; i < families.size(); ++i)
        {
            auto &family = families[i];

            if ((family.queueFlags & flags) != flags)
            {
                continue;
            }

            if (family.queueCount < queueCount)
            {
                continue;
            }

            return static_cast<int>(i);
        }

        return -1;
    }
};

class PhysicalDeviceExtensionCheckerImpl
{
public:
    static bool satisfies(const vk::raii::PhysicalDevice &device, const std::vector<const char *> &extensions)
    {
        auto deviceExtensions = device.enumerateDeviceExtensionProperties(nullptr);
        auto begin = deviceExtensions.begin();
        auto end = deviceExtensions.end();

        for (auto extension : extensions)
        {
            auto it = std::find_if(
                begin,
                end,
                [&](const vk::ExtensionProperties &ext) { return std::string(extension) == ext.extensionName; });
            if (it == end)
            {
                return false;
            }
        }

        return true;
    }
};
}

namespace engine
{
bool PhysicalDeviceFeaturesChecker::satisfies(
    const vk::raii::PhysicalDevice &device,
    const vk::PhysicalDeviceFeatures &features)
{
    return PhysicalDeviceFeaturesCheckerImpl::satisfies(device, features);
}

int QueueFamilySearcher::find(const vk::raii::PhysicalDevice &device, vk::QueueFlags flags, uint32_t queueCount)
{
    return QueueFamilySearcherImpl::find(device, flags, queueCount);
}

bool PhysicalDeviceExtensionChecker::satisfies(
    const vk::raii::PhysicalDevice &device,
    const std::vector<const char *> &extensions)
{
    return PhysicalDeviceExtensionCheckerImpl::satisfies(device, extensions);
}
}
