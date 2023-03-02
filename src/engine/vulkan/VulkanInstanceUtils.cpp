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

#include "VulkanInstanceUtils.h"

#include <engine/Version.h>

#include <iostream>
#include <unordered_set>

namespace
{
#ifndef NDEBUG
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
#endif

class ExtensionListSanitizer
{
public:
    static std::vector<std::string> sanitize(const std::vector<std::string> &input)
    {
        auto unique = std::unordered_set<std::string>(input.begin(), input.end());
        return std::vector<std::string>(unique.begin(), unique.end());
    }
};

#ifndef NDEBUG
class ExtensionListValidator
{
public:
    static bool valid(const std::vector<std::string> &extensions, const vk::raii::Context &context)
    {
        auto instanceExtensions = context.enumerateInstanceExtensionProperties(nullptr);

        for (auto &inputExtension : extensions)
        {
            auto it = std::find_if(
                instanceExtensions.begin(),
                instanceExtensions.end(),
                [&](const vk::ExtensionProperties &instanceExtension)
                {
                    auto &name = instanceExtension.extensionName;
                    return inputExtension == std::string(name.data());
                });

            if (it == instanceExtensions.end())
            {
                return false;
            }
        }

        return true;
    }
};
#endif

class ExtensionList
{
public:
    static ExtensionList create(const std::vector<std::string> &input)
    {
        auto uniques = ExtensionListSanitizer::sanitize(input);
        return ExtensionList(std::move(uniques));
    }

#ifndef NDEBUG
    static ExtensionList createDebug(std::vector<std::string> input, const vk::raii::Context &context)
    {
        input.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        assert(ExtensionListValidator::valid(input, context));
        return create(input);
    }
#endif

    std::vector<const char *> toCStringArray() const
    {
        auto result = std::vector<const char *>();
        result.reserve(_extensions.size());

        for (auto &extension : _extensions)
        {
            result.push_back(extension.c_str());
        }

        return result;
    }

private:
    ExtensionList(std::vector<std::string> extensions)
        : _extensions(std::move(extensions))
    {
    }

private:
    std::vector<std::string> _extensions;
};

class ApplicationInfoFactory
{
public:
    static vk::ApplicationInfo create()
    {
        auto version = VK_MAKE_API_VERSION(0, engine::Version::major, engine::Version::minor, engine::Version::patch);
        return vk::ApplicationInfo("RenderEngine", version, "RenderEngine", version, VK_API_VERSION_1_3);
    }
};

#ifndef NDEBUG
class DebugMessengerInfoFactory
{
public:
    static vk::DebugUtilsMessengerCreateInfoEXT create()
    {
        return vk::DebugUtilsMessengerCreateInfoEXT(
            vk::DebugUtilsMessengerCreateFlagsEXT{0u},
            _getSeverityFlags(),
            _getMessageTypeFlags(),
            VulkanDebugMessenger::onInput);
    }

private:
    static vk::DebugUtilsMessageSeverityFlagsEXT _getSeverityFlags()
    {
        using SeverityFlags = vk::DebugUtilsMessageSeverityFlagBitsEXT;
        return {SeverityFlags::eError | SeverityFlags::eInfo | SeverityFlags::eVerbose | SeverityFlags::eWarning};
    }

    static vk::DebugUtilsMessageTypeFlagsEXT _getMessageTypeFlags()
    {
        using TypeFlags = vk::DebugUtilsMessageTypeFlagBitsEXT;
        return {TypeFlags::eGeneral | TypeFlags::ePerformance | TypeFlags::eValidation};
    }
};
#endif

class InstanceFactory
{
public:
    static vk::raii::Instance create(const vk::raii::Context &context, const std::vector<std::string> &extensions)
    {
#ifndef NDEBUG
        return _createDebug(context, extensions);
#else
        return _create(context, extensions);
#endif
    }

private:
#ifndef NDEBUG
    static vk::raii::Instance _createDebug(const vk::raii::Context &context, const std::vector<std::string> &extensions)
    {
        auto validationLayers = std::vector<const char *>{"VK_LAYER_KHRONOS_validation"};
        auto appInfo = ApplicationInfoFactory::create();
        auto extensionList = ExtensionList::createDebug(extensions, context);
        auto cStringExtensionList = extensionList.toCStringArray();
        auto debugInfo = DebugMessengerInfoFactory::create();
        auto info = vk::InstanceCreateInfo(
            vk::InstanceCreateFlags{0u},
            &appInfo,
            validationLayers,
            cStringExtensionList,
            &debugInfo);
        return vk::raii::Instance(context, info);
    }
#endif

    static vk::raii::Instance _create(const vk::raii::Context &context, const std::vector<std::string> &extensions)
    {
        auto appInfo = ApplicationInfoFactory::create();
        auto extensionList = ExtensionList::create(extensions);
        auto cStringExtensionList = extensionList.toCStringArray();
        auto info = vk::InstanceCreateInfo(vk::InstanceCreateFlags{0u}, &appInfo, nullptr, cStringExtensionList);
        return vk::raii::Instance(context, info);
    }
};
}

namespace engine
{
vk::raii::Instance VulkanInstanceUtils::createInstance(
    const vk::raii::Context &context,
    const std::vector<std::string> &extensions)
{
    return InstanceFactory::create(context, extensions);
}
}
