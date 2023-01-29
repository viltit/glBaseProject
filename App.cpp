#include "App.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <thread>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "Camera2D.hpp"
#include "DrawableGL.hpp"
#include "Timer.hpp"
#include "MultiDrawable.hpp"
#include "FileReader.hpp"
#include "GLErrors.hpp"

using namespace  moe;

App::App(const std::vector<std::string>& args) :
    // TODO: Test hidpi window option
    _window { "hello Triangle", 0 ,0, moe::WindowStyle::fullscreen },
    _keyPressed (360, false)   // TODO: No magic numbers! (how to get number of SDL Key-Events?)
{
    // TODO: Some kind of argument testing, maybe map allowed arguments
    if (args.size() > 1) {
        if (args[1] == "debug") {
            spdlog::set_level(spdlog::level::debug);
        }
        else if (args[1] == "trace") {
            spdlog::set_level(spdlog::level::trace);
        }
        else if (args[1] == "info") {
            spdlog::set_level(spdlog::level::info);
        }
        else {
            spdlog::set_level(spdlog::level::warn);
        }
    }
    else {
            spdlog::set_level(spdlog::level::warn);
    }
    spdlog::info("Application is starting.");
}

App::~App() {
    spdlog::info("Application is ending.");
}

void App::loop() {

    try {
        Shader shader { 
            "Simple Shader",
            FileReader().toString("Shaders/Simple.vert"), 
            FileReader().toString("Shaders/Simple.frag") 
            };
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
                        _keyPressed[event.key.keysym.sym] = true;
                        switch(event.key.keysym.sym) {
                            case SDLK_ESCAPE:  
                                isRunning = false;
                                break;
                        }
                        break;
                    case SDL_KEYUP:
                        _keyPressed[event.key.keysym.sym] = false;
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
            updateInput(camera);

            try {
                shader.on();
                GLuint cameraUniform = shader.uniform("C");
                glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, glm::value_ptr(camera.matrix()));
                
                test.transform.rotate(0.1, glm::vec3{ 0, 0, 1});
                for (auto& position : positions) {
                    position = glm::rotate(position, -3.f / 360.f, glm::vec3{ 0 , 0, 1 });
                }
                test.update(positions);
                test.draw(shader);
                /* for (const auto& drawable : scene) {
                    drawable.draw(shader);
                } */
                shader.off();

                // TODO: poll errors from gl
                GLenum err;
                while((err = glGetError()) != GL_NO_ERROR) {
                    spdlog::warn("Drawing loop stumbled on gl-error " + err);
                }
            }
            catch (ShaderError e) {
                spdlog::warn(e.what());
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
    catch(ShaderCompileError e) {
        spdlog::error(e.what());
        spdlog::error(e.log);
    }
    catch(std::runtime_error e) {
        spdlog::critical(e.what());
    }
}

void App::updateInput(Camera2D& camera) {

    if (_keyPressed[SDLK_a]) {
        camera.translate(glm::vec3(5, 0, 0));
    }
    if (_keyPressed[SDLK_d]) {
        camera.translate(glm::vec3(-5, 0, 0));
    }
    if (_keyPressed[SDLK_w]) {
        camera.translate(glm::vec3(0, -5, 0));
    }
    if (_keyPressed[SDLK_s]) {
        camera.translate(glm::vec3(0, 5, 0));
    }
}
