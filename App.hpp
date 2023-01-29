#pragma once

#include <string>
#include <vector>

#include "Window.hpp"

/**
 * TODO: This class should controll the current screen, ie MainMenu, Game, Editor, ...
*/
class App {

public:
    App(const std::vector<std::string>& args);
    ~App();

    void loop();

private:
    moe::Window _window;
};