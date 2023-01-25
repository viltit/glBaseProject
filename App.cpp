#include "App.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <thread>

#include "Camera2D.hpp"
#include "DrawableGL.hpp"
#include "Timer.hpp"
#include "MultiDrawable.hpp"

using namespace  moe;

// TODO: Read shader from file
const std::string vertexShader = 
    "#version 400 core \n"
    "layout (location = 0) in vec3 v_pos; \n"
    "layout (location = 1) in vec4 v_color; \n"
    "layout (location = 2) in mat4 T; \n"
    "layout (location = 6) in vec4 color; \n"
    "\n"
    "out vec4 fragColor; \n"
    "uniform mat4 C; \n"
    "uniform mat4 M; \n"
    "\n"
    "void main() \n"
    "{ \n"
    "    fragColor = color; \n"
    "    gl_Position = C * M * T * vec4(v_pos, 1.0); \n"
    "}";

const std::string fragmentShader = 
    "#version 400 core \n"
    "\n"
    "in      vec4    fragColor; \n"
    "out		vec4	color; \n"

    "void main() { \n"
    "    color = fragColor; \n"
    "}";


App::App() :
    // TODO: Test hidpi window option
    _window { "hello Triangle", 0 ,0, moe::WindowStyle::fullscreen },
    _shader { vertexShader, fragmentShader }
{ }

void App::loop() {

    Timer timer { };
    Camera2D camera { glm::vec2{ _window.width(), _window.height() }, glm::vec3{ 0, 0, -1 } };

    // wip: use instanced drawing
    auto color = glm::vec4{ 1, 0, 0, 1 };
    auto vertices = std::vector<Vertex> {
                    Vertex{ glm::vec3(-100, -100, 0.0), color },
                    Vertex{ glm::vec3(100, -100, 0.0), color },
                    Vertex{ glm::vec3(-100,  100, 0.0), color },
                    Vertex{ glm::vec3(-100,  100, 0.0), color },
                    Vertex{ glm::vec3(100, -100, 0.0), color },
                    Vertex{ glm::vec3(100, 100, 0.0), color } };
    
    auto positions = std::vector<glm::mat4>();
    auto colors = std::vector<glm::vec4>();
    positions.reserve(100000);
    colors.reserve(10000);
    for (int i = 0; i < 100000; i++) {
        
        float x = (i / 100.f) * 300.f - 7500.f;
        float y = (i % 100) * 300.f - 7500.f;
        float z = (float)(rand()) / (float)(RAND_MAX) * 2.f - 1.f;
        float scale = (float)(rand()) / (float)(RAND_MAX) * 1.3f + 0.2f;
        float rotation = (float)(rand()) / (float)(RAND_MAX) * 360.f;
        
        glm::vec3 translation = glm::vec3{ x, y , z };
        Transform transform{ translation, glm::vec3{ scale, scale, scale } };
        transform.rotate(rotation, glm::vec3{ 0.f, 0.f, 1.f });

        positions.push_back(transform.worldMatrix());

        float r = (float)(rand()) / (float)(RAND_MAX);
        float g = (float)(rand()) / (float)(RAND_MAX);
        float b = (float)(rand()) / (float)(RAND_MAX);
        float a = (float)(rand()) / (float)(RAND_MAX) * 0.8f + 0.2f;
        colors.push_back(glm::vec4{ r, g, b, a});
    }

    auto test = MultiDrawableGL { 
        vertices,
        positions,
        colors,
        glm::vec3{ 1 }
    };

    // ugly Test: create 2500 drawables
    auto scene { std::vector<DrawableGL>() };
    scene.reserve(2500);
    for (int i = 0; i < 2500; i++) {
        float x = (i / 50.f) * 300.f - 7500.f;
        float y = (i % 50) * 300.f - 7500.f;
        float z = (float)(rand()) / (float)(RAND_MAX) * 2.f - 1.f;
        float r = (float)(rand()) / (float)(RAND_MAX);
        float g = (float)(rand()) / (float)(RAND_MAX);
        float b = (float)(rand()) / (float)(RAND_MAX);
        float a = (float)(rand()) / (float)(RAND_MAX) * 0.8f + 0.2f;
        
        if (i % 2 == 1) {
            scene.push_back(Rectangle(glm::vec4{ r, g, b, a }, glm::vec3{ x, y, z }));
        }
        else {
            scene.push_back(Triangle(glm::vec4{ r, g, b, a }, glm::vec3{ x, y, z }));
        }
        float scale = (float)(rand()) / (float)(RAND_MAX) * 1.3f + 0.2f;
        float rotation = (float)(rand()) / (float)(RAND_MAX) * 360.f;
        scene[i].transform.setScale(glm::vec3{ scale, scale, scale });
        scene[i].transform.rotate(rotation, glm::vec3{ 0.f, 0.f, 1.f });
    }


    bool isRunning = true;
    SDL_Event event;
    timer.start();
    
    // TODO: Run loop in a thread ? Run a render and a world update thread ?
    while (isRunning) {
        
        _window.clear();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                case SDL_APP_TERMINATING:
                    isRunning = false;
                    break;
                // TODO: Window resize
                // TODO: Handle keyboard and mouse inputs in a dedicated class without hardcoded key-bindings
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE:  
                            isRunning = false;
                            break;
                        case SDLK_a:
                            camera.translate(glm::vec3(5, 0, 0));
                            break;
                        case SDLK_d:
                            camera.translate(glm::vec3(-5, 0, 0));
                            break;
                        case SDLK_w:
                            camera.translate(glm::vec3(0, -5, 0));
                            break;
                        case SDLK_s:
                            camera.translate(glm::vec3(0, 5, 0));
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0) {
                        camera.zoomIn(0.1f);
                    }
                    else if (event.wheel.y < 0) {
                        camera.zoomOut(0.1f);
                    }
                    break;
            }
        }


        camera.update();

        _shader.on();
        GLuint cameraUniform = _shader.uniform("C");
        glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, glm::value_ptr(camera.matrix()));
        
        test.transform.rotate(0.1, glm::vec3{ 0, 0, 1});
        for (auto& position : positions) {
            position = glm::rotate(position, -3.f / 360.f, glm::vec3{ 0 , 0, 1 });
        }
        test.update(positions);
        test.draw(_shader);
        /* for (const auto& drawable : scene) {
            drawable.draw(_shader);
        } */
        _shader.off();

        // TODO: poll errors from gl
        if (glGetError() != 0) {
            std::cout << "GL reported an error." << std::endl;
        }
        _window.swap();

        // TODO: Fetch screens refresh rate
        // TODO: Calculate FPS -> needs a second timer or frame counter because we do not want to calculate fps every frame
        int deltaTime = timer.delta();
        while (deltaTime < 17) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            deltaTime += timer.delta();
        } 
    }
}