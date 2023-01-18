#include "../dependencies/glm/mat4x4.hpp"
#include <iostream>

int main() {
    glm::mat4 m({1, 2, 3, 4}, 
                {5, 6, 7, 8}, 
                {9, 3, 2, 2}, 
                {5, 4, 4, 1});

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            std::cout << m[i][j] << " ";
        }

        std::cout << std::endl;
    }
}
