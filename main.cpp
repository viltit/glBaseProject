#include "App.hpp"
#include <iostream>

int main(int argc, char** argv) {

    try {
        App app = App();
        app.loop();

        return 0;
    }
    catch(std::exception e) {
        std::cout << e.what() << std::endl;
    }
}