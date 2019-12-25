# Environment Variables
export SDK_PATH=/Users/aidan/esp8266/esp-open-rtos
export ESPPORT=/dev/tty.SLAB_USBtoUART
export PROJECT_ROOT=`pwd`

# Build functions
build() {
    python $PROJECT_ROOT/tools/gen_product.py
    docker run -it --rm -v `pwd`:/project -w /project esp-rtos make all 
}

install() {
    make erase_flash flash monitor
}

monitor() {
    make monitor
}
