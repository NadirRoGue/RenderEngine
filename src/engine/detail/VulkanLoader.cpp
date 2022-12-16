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

#include "VulkanLoader.h"

#include <dlfcn.h>

#include <stdexcept>
#include <string>

namespace engine::detail
{
VulkanLoader::VulkanLoader()
{
    _loadLib();
    _loadExportedFunctions();
    _loadGlobalFunctions();
}

VulkanLoader::~VulkanLoader()
{
    dlclose(_vulkan);
}

void VulkanLoader::_loadLib()
{
    _vulkan = dlopen("libvulkan.so.1", RTLD_NOW);
    if (!_vulkan)
    {
        throw std::runtime_error("Cannot load vulkan library");
    }
}

void VulkanLoader::_loadExportedFunctions()
{
#define EXPORTED_VULKAN_FUNCTION(name) \
    name = (PFN_##name)dlsym(_vulkan, #name); \
    if (!name) \
    { \
        throw std::runtime_error("Could not load " + std::string(#name)); \
    }

#include <engine/VulkanFunctions.inl>
}

void VulkanLoader::_loadGlobalFunctions()
{
#define GLOBAL_VULKAN_FUNCTION(name) \
    name = (PFN_##name)vkGetInstanceProcAddr(nullptr, #name); \
    if (!name) \
    { \
        throw std::runtime_error("Could not load " + std::string(#name)); \
    }

#include <engine/VulkanFunctions.inl>
}

void VulkanLoader::loadInstanceFunctions(VkInstance instance)
{
#define INSTANCE_VULKAN_FUNCTION(name) \
    name = (PFN_##name)vkGetInstanceProcAddr(instance, #name); \
    if (!name) \
    { \
        throw std::runtime_error("Could not load " + std::string(#name)); \
    }

#include <engine/VulkanFunctions.inl>
}

void VulkanLoader::loadDeviceFunctions(VkDevice device)
{
}
}
