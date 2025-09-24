
SAMPLE=init_functions

rm -rf build/
cmake -S . -B build/ -DMICROS_BOARD=lm/lm3s6965evb -DMICROS_SAMPLE=${SAMPLE} -DCMAKE_BUILD_TYPE=Debug
cmake --build build/
arm-none-eabi-size build/samples/${SAMPLE}/${SAMPLE}

qemu-system-arm -M lm3s6965evb -nographic -kernel build/samples/${SAMPLE}/${SAMPLE}
