# Environment Variables
export SDK_PATH=`pwd`/components/esp-open-rtos
export ESPPORT=/dev/tty.SLAB_USBtoUART
export PROJECT_ROOT=`pwd`

# Build functions
build() {
    git submodule update --init --recursive
    if [ ! -f tools/.sdk-built ]; then
        docker build . -f tools/esp-sdk.dockerfile -t esp-sdk
        touch tools/.sdk-built
    fi
    if [ ! -f tools/.rtos-built ]; then
        docker build . -f tools/esp-rtos.dockerfile -t esp-rtos
        touch tools/.rtos-built
    fi
    python $PROJECT_ROOT/tools/gen_product.py
    docker run -it --rm -v `pwd`:/project -w /project esp-rtos make all 
}

clean() {
    rm -rf $PROJECT_ROOT/build $PROJECT_ROOT/firmware $PROJECT_ROOT/product.h
}

install() {
    make erase_flash flash monitor
}

monitor() {
    make monitor
}
