# Anirenderer
Anirenderer is a 3D software renderer that uses the CPU for all calculations to render 3D objects. The main advantage over classic GPU rendering is that the rendering results are reproducible across different hardware.

# Screenshots
![](/latex/backpack_far.png?raw=true "Backpack from afar")

![](/latex/backpack_light.png?raw=true "Backpack light side")

![](/latex/backpack_dark.png?raw=true "Backpack dark side")

The screenshots above show a render of a backpack illuminated from one side by a point light originating from a white cube.

# Build and run 
## Linux
    mkdir build
    cmake -DCMAKE_BUILD_TYPE=Release -S . -B build 
    cd build && make anirenderer 
    ./anirenderer

# Test program

The compiled binary includes a test program that allows you to explore a 3D scene. You can move using the WASD keys and look around using your mouse. Press the "C" key to switch between scene exploration mode and mouse usage mode.
