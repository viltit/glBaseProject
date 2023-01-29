#include "App.hpp"

#include <string>
#include <vector>

int main(int argc, char** argv) {

    std::vector<std::string> arguments { };
    for (int i = 0; i < argc; i++) {
        arguments.push_back(argv[i]);
    }
    App app = App(arguments);
    app.loop();
    return 0;
}