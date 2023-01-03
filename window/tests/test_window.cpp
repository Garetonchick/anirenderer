#include <window/window.h>
#include <thread>

int main()
{
    using namespace std::chrono_literals;

    ani::Window window(500, 500);

    ani::Image image{.pixels = std::vector<std::vector<ani::RGB>>(256, std::vector<ani::RGB>(256, {0, 0, 0}))};

    for(size_t i = 0; i < 256; ++i) {
        for(size_t j = 0; j < 256; ++j) {
            if(((i / 8) & 1) + ((j / 8) & 1) == 1) {
                image.pixels[i][j] = {255, 255, 255};
            }
        }
    }

    window.Display(image);

    std::this_thread::sleep_for(4s); 

    window.Display(image);

    std::this_thread::sleep_for(6s); 

    return 0;
}

