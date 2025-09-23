
rm -rf build/
cmake -S . -B build/ -DMICROS_BOARD=lm/lm3s6965evb -DMICROS_SAMPLE=hello_world -DCMAKE_BUILD_TYPE=Debug
cmake --build build/
arm-none-eabi-size build/samples/hello_world/hello_world

qemu-system-arm -M lm3s6965evb -nographic -kernel build/samples/hello_world/hello_world
