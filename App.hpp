#pragma once

#include <string>
#include <vector>

#include "Window.hpp"
#include "Camera2D.hpp"

/**
 * TODO: This class should controll the current screen, ie MainMenu, Game, Editor, ...
*/
class App {

public:
    App(const std::vector<std::string>& args);
    ~App();

    void loop();

private:

    void updateInput(moe::Camera2D& camera);
    moe::Window _window;
    std::vector<bool> _keyPressed;
};