#include <application/application.h>

#include <iostream>
#include <exception>

int main() {
    ani::Application app;

    try {
        app.Run();
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch(...) {
        std::cerr << "Unexpected exception was thrown" << std::endl;
        return 2;
    }

    return 0;
}
