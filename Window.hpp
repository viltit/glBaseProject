#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <memory>

#include "Color.hpp"

namespace moe {

    enum WindowStyle {

        none        = 0x00,
        fullscreen  = SDL_WINDOW_FULLSCREEN,         /**< fullscreen window */
        opengl      = SDL_WINDOW_OPENGL,             /**< window usable with OpenGL context */
        visible     = SDL_WINDOW_SHOWN,              /**< window is visible */
        hidden      = SDL_WINDOW_HIDDEN,             /**< window is not visible */
        borderless  = SDL_WINDOW_BORDERLESS,         /**< no window decoration */
        resizable   = SDL_WINDOW_RESIZABLE,          /**< window can be resized */
        minimized   = SDL_WINDOW_MINIMIZED,          /**< window is minimized */
        maximized   = SDL_WINDOW_MAXIMIZED,          /**< window is maximized */
        inputGrabbed = SDL_WINDOW_INPUT_GRABBED,      /**< window has grabbed input focus */
        inputFocus  = SDL_WINDOW_INPUT_FOCUS,        /**< window has input focus */
        mouseFocus  = SDL_WINDOW_MOUSE_FOCUS,        /**< window has mouse focus */
        fullscreenDesktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
        // SDL_WINDOW_FOREIGN = 0x00000800,          /**< window not created by SDL */
        highdpi     = SDL_WINDOW_ALLOW_HIGHDPI,      /**< window should be created in high-DPI mode if supported.
                                                        On macOS NSHighResolutionCapable must be set true in the
                                                        application's Info.plist for this to have any effect. */
        mouseCapture = SDL_WINDOW_MOUSE_CAPTURE,     /**< window has mouse captured (unrelated to INPUT_GRABBED) */
        onTop       = SDL_WINDOW_ALWAYS_ON_TOP,      /**< window should always be above others */
        // SDL_WINDOW_SKIP_TASKBAR  = 0x00010000,    /**< window should not be added to the taskbar */
        utility     = SDL_WINDOW_UTILITY,            /**< window should be treated as a utility window */
        tooltip     = SDL_WINDOW_TOOLTIP,            /**< window should be treated as a tooltip */
        popup       = SDL_WINDOW_POPUP_MENU,         /**< window should be treated as a popup menu */
        // SDL_WINDOW_VULKAN        = 0x10000000     /**< window usable for Vulkan surface */
    };


    class Window {
    public:
        Window(const std::string& title, size_t width, size_t height,
                WindowStyle style = WindowStyle::none,
                Color background = Color::black());

        virtual ~Window();

        virtual void clear() const;     // clear the actual screen
        virtual void swap() const;      // swap screen (we have  double-buffer)

        size_t width() const    { return _w; }
        size_t height() const   { return _h; }

    protected:
        SDL_Window* _window;
        size_t _w;
        size_t _h;
    };
}
