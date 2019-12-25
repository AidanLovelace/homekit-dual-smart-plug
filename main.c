#include <stdio.h>
#include <espressif/esp_wifi.h>
#include <espressif/esp_sta.h>
#include <esp/uart.h>
#include <esp8266.h>
#include <FreeRTOS.h>
#include <task.h>


#include "product.h"
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <wifi_config.h>
#include "wifi.h"

const int top_gpio = 14;
bool top_on = false;
const int bottom_gpio = 12;
bool bottom_on = false;

void top_write(bool on) {
    gpio_write(top_gpio, on ? 0 : 1);
}

void top_init() {
    gpio_enable(top_gpio, GPIO_OUTPUT);
    top_write(top_on);
}

homekit_value_t top_on_get() {
    return HOMEKIT_BOOL(top_on);
}

void top_on_set(homekit_value_t value) {
    if (value.format != homekit_format_bool) {
        printf("Invalid value format: %d\n", value.format);
        return;
    }

    top_on = value.bool_value;
    top_write(top_on);
}

void bottom_write(bool on) {
    gpio_write(bottom_gpio, on ? 0 : 1);
}

void bottom_init() {
    gpio_enable(bottom_gpio, GPIO_OUTPUT);
    bottom_write(bottom_on);
}

homekit_value_t bottom_on_get() {
    return HOMEKIT_BOOL(bottom_on);
}

void bottom_on_set(homekit_value_t value) {
    if (value.format != homekit_format_bool) {
        printf("Invalid value format: %d\n", value.format);
        return;
    }

    bottom_on = value.bool_value;
    bottom_write(bottom_on);
}

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[]){
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]){
            HOMEKIT_CHARACTERISTIC(NAME, PRODUCT_FULL_NAME),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, MANUFACTURER),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, SERIAL_NUMBER),
            HOMEKIT_CHARACTERISTIC(MODEL, PRODUCT_NAME),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.1"),
            NULL
        }),
        HOMEKIT_SERVICE(OUTLET, .primary=true, .characteristics=(homekit_characteristic_t*[]){
            HOMEKIT_CHARACTERISTIC(NAME, "Top Outlet"),
            HOMEKIT_CHARACTERISTIC(
                ON, false,
                .getter=top_on_get,
                .setter=top_on_set
            ),
            NULL
        }),
        HOMEKIT_SERVICE(OUTLET, .primary=true, .characteristics=(homekit_characteristic_t*[]){
            HOMEKIT_CHARACTERISTIC(NAME, "Bottom Outlet"),
            HOMEKIT_CHARACTERISTIC(
                ON, false,
                .getter=bottom_on_get,
                .setter=bottom_on_set
            ),
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = PASSWORD,
    .setupId = PRODUCT_4_ID,
};

void on_wifi_event(wifi_config_event_t event) {
    if (event == WIFI_CONFIG_CONNECTED) {
        printf("Connected to WiFi\n");
        homekit_server_init(&config);
    } else if (event == WIFI_CONFIG_DISCONNECTED) {
        printf("Disconnected from WiFi\n");
    }
}

void user_init(void) {
    uart_set_baud(0, 115200);

    wifi_init();
    top_init();
    bottom_init();
    wifi_config_init2(PRODUCT_FULL_NAME, PASSWORD, on_wifi_event);
}
