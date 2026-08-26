#include <stdio.h>
#include "pico/stdlib.h"

int main() {
const uint32_t bit = 1u << PICO_DEFAULT_LED_PIN;
    gpio_init(PICO_DEFAULT_LED_PIN);
    sio_hw->gpio_oe_set = bit; //habilita la salida
    while (true) {
        sio_hw->gpio_set = bit; //enciende el led (alto) 
        sleep_ms(500);
        sio_hw->gpio_clr = bit; //apaga el led (bajo)
        sleep_ms(500);
    }
}