#include "Window.hpp"

#include "GLErrors.hpp"
#include "Color.hpp"

#include <iostream>
#include <GL/glew.h>

namespace moe {

Window::Window(const std::string& title, size_t width, size_t height, WindowStyle style, Color background)
    :   _window     { nullptr },
        _h          { height },
        _w          { width }
{

    // put together the desired window style with bitwise ops
    auto windowOptions = SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED;
    windowOptions |= style;

    // TODO: Initialize SDL in a spearate class?
    // TODO: Support SDL_AUDIO?
    if (SDL_Init(SDL_INIT_VIDEO) > 1) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        throw InitError("Could not initialize SDL.");
    }

    //if the window is fullsize, adjust width and height:
    if (windowOptions & SDL_WINDOW_FULLSCREEN || windowOptions & SDL_WINDOW_MAXIMIZED) {
        //TODO: Maximized != Fullscreen !!
        SDL_DisplayMode resolution;
        SDL_GetDisplayMode(0, 0, &resolution);
        _w = resolution.w;
        _h = resolution.h;
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    _window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            _w, _h,
            windowOptions);
    if (!_window) {
        std::cout << SDL_GetError();
        throw InitError("Could not initialize a Window.");
    }

    if (SDL_GL_CreateContext(_window) == nullptr) {
        throw InitError("Window could not create an OpenGL-Context.");
    }

    if (glewInit() != GLEW_OK) {
        throw InitError("Could not initialize OpenGL.");
    }

    // TODO: Parameterize
    glClearColor(background.r, background.g, background.b, 1.0);
    glClearDepth(1.0);
    glViewport(0, 0, _w, _h);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_MULTISAMPLE); 

    if (glGetError() != 0)  {
        throw InitError("GL initialized with errors.");
    }
}

Window::~Window() {
    SDL_DestroyWindow(_window);
    _window = nullptr;
    SDL_Quit();
}

void Window::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swap() const {
    SDL_GL_SwapWindow(_window);
}

}
