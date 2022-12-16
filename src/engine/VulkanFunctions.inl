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

// From "Vulkan Cookbook"

#ifndef EXPORTED_VULKAN_FUNCTION
    #define EXPORTED_VULKAN_FUNCTION(function)
#endif

EXPORTED_VULKAN_FUNCTION(vkGetInstanceProcAddr)

#undef EXPORTED_VULKAN_FUNCTION

#ifndef GLOBAL_VULKAN_FUNCTION
    #define GLOBAL_VULKAN_FUNCTION(function)
#endif

GLOBAL_VULKAN_FUNCTION(vkEnumerateInstanceExtensionProperties)
GLOBAL_VULKAN_FUNCTION(vkEnumerateInstanceLayerProperties)
GLOBAL_VULKAN_FUNCTION(vkCreateInstance)

#undef GLOBAL_VULKAN_FUNCTION

#ifndef INSTANCE_VULKAN_FUNCTION
    #define INSTANCE_VULKAN_FUNCTION(function)
#endif

INSTANCE_VULKAN_FUNCTION(vkDestroyInstance)
INSTANCE_VULKAN_FUNCTION(vkDestroySurfaceKHR)

#ifdef _WIN32
INSTANCE_VULKAN_FUNCTION(vkCreateXlibSurfaceKHR)
#else
INSTANCE_VULKAN_FUNCTION(vkCreateXlibSurfaceKHR)
#endif

#undef INSTANCE_VULKAN_FUNCTION

#ifndef INSTANCE_EXTENSION_VULKAN_FUNCTION
    #define INSTANCE_EXTENSION_VULKAN_FUNCTION(function, extension)
#endif

#undef INSTANCE_EXTENSION_VULKAN_FUNCTION

#ifndef DEVICE_VULKAN_FUNCTION
    #define DEVICE_VULKAN_FUNCTION(function)
#endif

#undef DEVICE_VULKAN_FUNCTION

#ifndef DEVICE_EXTENSION_VULKAN_FUNCTION
    #define DEVICE_EXTENSION_VULKAN_FUNCTION(function, extension)
#endif

#undef DEVICE_EXTENSION_VULKAN_FUNCTION
