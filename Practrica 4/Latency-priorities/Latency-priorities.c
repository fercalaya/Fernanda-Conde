#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED_PIN 15 //led externo
#define BTN_PIN 16 //boton con pull-up externo, pulsador a GND
#define TRAZA_PIN 14 //pin dedicado a la traza de la ISR (Canal 2)

static void button_isr(uint gpio, uint32_t events) {
    //sube la traza al inicio de la ISR
    gpio_put(TRAZA_PIN, 1);
    //verifica si la interrupción fue por flanco de bajada en el boton
    if (gpio == BTN_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        gpio_xor_mask(1u << LED_PIN);
    }
    //limpia la bandera de interrupción
    gpio_acknowledge_irq(gpio, events);
    //baja la traza al finalizar la ISR
    gpio_put(TRAZA_PIN, 0);
}

int main(void) {
    stdio_init_all();
    //led como salida
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); //arrancar en estado conocido (apagado)
    //pin de traza como salida
    gpio_init(TRAZA_PIN);
    gpio_set_dir(TRAZA_PIN, GPIO_OUT);
    gpio_put(TRAZA_PIN, 0); //en bajo cuando no estamos en la ISR
    //boton como entrada
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_disable_pulls(BTN_PIN); //deshabilita pull interno (se usa resistencia externa)
    //configura e inicia la interrupcion
    gpio_set_irq_enabled_with_callback(
        BTN_PIN,
        GPIO_IRQ_EDGE_FALL,
        true,
        &button_isr
    );
    while (true) {
        tight_loop_contents();
    }
}