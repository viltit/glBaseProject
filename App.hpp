#include "Window.hpp"
#include "Shader.hpp"

class App {

public:
    App();

    void loop();

private:
    moe::Window _window;
    // TODO: Shader should be bound to groups of drawable shapes
    moe::Shader _shader;
};