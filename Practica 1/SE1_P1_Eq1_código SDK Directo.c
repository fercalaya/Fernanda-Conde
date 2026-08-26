#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1); //enciende el led (alto)
        sleep_ms(500);
        gpio_put(LED_PIN, 0); //apaga el led (bajo)
        sleep_ms(500);
    }
}