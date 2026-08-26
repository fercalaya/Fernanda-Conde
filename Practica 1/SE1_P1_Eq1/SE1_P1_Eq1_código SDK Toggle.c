#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_xor_mask(1u << LED_PIN); // hace toggle del pin mediante SDK
        sleep_ms(500);
    }
}