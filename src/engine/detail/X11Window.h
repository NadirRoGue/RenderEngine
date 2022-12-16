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

#include <engine/VulkanFunctions.h>

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>

#include <string_view>

namespace engine::detail
{
class X11Surface
{
public:
    X11Surface(VkInstance instance, Display *display, Window window);
    ~X11Surface();

private:
    VkInstance _instance;
    VkSurfaceKHR _surface;
};

class X11Window
{
public:
    X11Window(VkInstance instance, std::string_view title, uint16_t width, uint16_t height);
    ~X11Window();

    template<typename Callable>
    void renderLoop(Callable &&callable)
    {
        const Atom deleteWindowEvent = XInternAtom(_display, "WM_DELETE_WINDOW", False);
        while (true)
        {
            callable();

            auto event = XEvent();
            XNextEvent(_display, &event);

            if (event.type == ClientMessage
                && (static_cast<unsigned int>(event.xclient.data.l[0]) == deleteWindowEvent))
            {
                break;
            }
        }
    }

private:
    Display *_display;
    Window _window;
    X11Surface _surface;
};
};
