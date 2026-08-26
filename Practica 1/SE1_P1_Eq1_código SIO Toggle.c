#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    const uint32_t bit = 1u << PICO_DEFAULT_LED_PIN;
    gpio_init(PICO_DEFAULT_LED_PIN);
    sio_hw->gpio_oe_set = bit; //habilita la salida

    while (true) {
        sio_hw->gpio_togl = bit; //invierte automaticamente la función incial
        sleep_ms(500);
    }
}