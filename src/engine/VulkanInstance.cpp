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

#include "VulkanInstance.h"

#include <engine/Version.h>

#include <cassert>
#include <stdexcept>
#include <vector>

#include <iostream>

namespace
{
class VulkanDebugMessenger
{
public:
    static VKAPI_ATTR VkBool32 VKAPI_CALL onInput(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }
};

class VulkanInstanceFactory
{
public:
    static VkInstance create(std::string_view applicationName)
    {
        auto appInfo = _createApplicationInfo(applicationName);

#ifndef NDEBUG
        auto debugMessenger = _createDebugMessenger();
#endif

        auto instanceInfo = VkInstanceCreateInfo{
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#ifndef NDEBUG
            &debugMessenger,
#else
            nullptr,
#endif
            0,
            &appInfo,
#ifndef NDEBUG
            static_cast<uint32_t>(_validationLayers.size()),
            _validationLayers.data(),
#else
            0,
            nullptr,
#endif
            static_cast<uint32_t>(_instanceExtensions.size()),
            _instanceExtensions.data()};

        auto instance = VkInstance();
        auto result = engine::vkCreateInstance(&instanceInfo, nullptr, &instance);

        if (result != VK_SUCCESS || instance == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Could not create vulkan instance");
        }

        return instance;
    }

private:
    static VkApplicationInfo _createApplicationInfo(std::string_view applicationName)
    {
        auto version = VK_MAKE_VERSION(engine::Version::major, engine::Version::minor, engine::Version::patch);
        return VkApplicationInfo{
            VK_STRUCTURE_TYPE_APPLICATION_INFO,
            nullptr,
            applicationName.data(),
            version,
            "RenderEngine",
            version,
            VK_MAKE_VERSION(1, 3, 231)};
    }

    inline static const std::vector<const char *> _instanceExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
        "VK_KHR_win32_surface"
#else
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
#ifndef NDEBUG
        ,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
    };

#ifndef NDEBUG
    inline static const std::vector<const char *> _validationLayers = {"VK_LAYER_KHRONOS_validation"};
#endif

    static VkDebugUtilsMessengerCreateInfoEXT _createDebugMessenger()
    {
        auto createInfo = VkDebugUtilsMessengerCreateInfoEXT{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = VulkanDebugMessenger::onInput;
        return createInfo;
    }
};
}

namespace engine
{
VulkanInstance::VulkanInstance(std::string_view applicationName)
    : _instance(VulkanInstanceFactory::create(applicationName))
{
    _loader.loadInstanceFunctions(_instance);
}

VulkanInstance::~VulkanInstance()
{
    vkDestroyInstance(_instance, nullptr);
}
}
