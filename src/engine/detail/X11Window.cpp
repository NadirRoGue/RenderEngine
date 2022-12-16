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

#include "WindowImpl.h"

#include <cassert>
#include <cstring>
#include <stdexcept>

namespace
{
class SurfaceFactory
{
public:
    static VkSurfaceKHR create(VkInstance instance, Display *display, Window window)
    {
        VkResult err;
        auto surfaceCreateInfo = VkXlibSurfaceCreateInfoKHR();

        std::memset(&surfaceCreateInfo, 0, sizeof(surfaceCreateInfo));
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.dpy = display;
        surfaceCreateInfo.window = window;

        auto surface = VkSurfaceKHR();
        auto result = engine::vkCreateXlibSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Could not create X11 surface");
        }

        return surface;
    }
};

class DisplayFactory
{
public:
    static Display *create()
    {
        auto display = XOpenDisplay(nullptr);
        assert(display);
        return display;
    }
};

class WindowFactory
{
public:
    static Window create(Display *display, uint16_t width, uint16_t height)
    {
        auto screen = DefaultScreen(display);
        auto window = RootWindow(display, screen);
        auto x = 0;
        auto y = 0;
        auto iWidth = static_cast<int>(width);
        auto iHeight = static_cast<int>(height);
        auto borderWidth = 1;
        auto borderColor = BlackPixel(display, screen);
        auto backgroundColor = WhitePixel(display, screen);
        return XCreateSimpleWindow(display, window, x, y, iWidth, iHeight, borderWidth, borderColor, backgroundColor);
    }
};

class WindowConfig
{
public:
    static void apply(Display *display, Window window, std::string_view title)
    {
        XSelectInput(display, window, ExposureMask | KeyPressMask);
        XMapWindow(display, window);
        XStoreName(display, window, title.data());
        auto deleteWindowEvent = XInternAtom(display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(display, window, &deleteWindowEvent, 1);
    }
};
}

namespace engine::detail
{
X11Surface::X11Surface(VkInstance instance, Display *display, Window window)
    : _instance(instance)
    , _surface(SurfaceFactory::create(instance, display, window))
{
}

X11Surface::~X11Surface()
{
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
}

X11Window::X11Window(VkInstance instance, std::string_view title, uint16_t width, uint16_t height)
    : _display(DisplayFactory::create())
    , _window(WindowFactory::create(_display, width, height))
    , _surface(instance, _display, _window)
{
    WindowConfig::apply(_display, _window, title);
}

X11Window::~X11Window()
{
    XDestroyWindow(_display, _window);
    XCloseDisplay(_display);
}
}
