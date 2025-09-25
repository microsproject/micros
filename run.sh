SAMPLE=$1
RUN_QEMU=$2

if [ -z "$SAMPLE" ]; then
    echo "Error: No target specified."
    exit 1
fi

rm -rf build/
cmake -S . -B build/ -DMICROS_BOARD=lm/lm3s6965evb -DMICROS_SAMPLE=${SAMPLE} -DCMAKE_BUILD_TYPE=Debug -GNinja
cmake --build build/

if [ "$RUN_QEMU" = "run" ]; then
    qemu-system-arm -M lm3s6965evb -nographic -kernel build/samples/${SAMPLE}/${SAMPLE}.elf
fi