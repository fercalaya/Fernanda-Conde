#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

#define ALARM_NUM 0 //define el numero de alarma
#define ALARM_IRQ TIMER_IRQ_0 //define la interrupcion del timer
#define INTERVAL_US 500000 //define el intervalo de tiempo en microsegundos
#define TRACE_PIN 14 //define el gpio para la traza del osciloscopio
#define LED_PIN 15 //define el pin del led externo
volatile uint32_t next_alarm; //guarda el siguiente tiempo de alarma

void on_alarm_irq() {
    hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM); //limpia la interrupcion de la alarma
    gpio_xor_mask(1u << LED_PIN); //cambia el estado del led externo
    gpio_xor_mask(1u << TRACE_PIN); //cambia el estado del gpio14
    next_alarm += INTERVAL_US; //calcula el siguiente tiempo de alarma
    timer_hw->alarm[ALARM_NUM] = next_alarm; //programa la siguiente alarma
}

int main() {
    stdio_init_all(); //inicia la comunicacion serial
    gpio_init(LED_PIN); //inicia el pin del led externo
    gpio_set_dir(LED_PIN, GPIO_OUT); //configura el led como salida
    gpio_put(LED_PIN, 0); //inicia el led apagado
    gpio_init(TRACE_PIN); //inicia el pin de la traza
    gpio_set_dir(TRACE_PIN, GPIO_OUT); //configura el gpio14 como salida
    gpio_put(TRACE_PIN, 0); //inicia la traza en bajo
    irq_set_exclusive_handler(ALARM_IRQ, on_alarm_irq); //asigna la funcion a la interrupcion
    hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM); //habilita la alarma del timer
    irq_set_enabled(ALARM_IRQ, true); //habilita la interrupcion en el nucleo

    next_alarm = timer_hw->timerawl + INTERVAL_US; //calcula el primer tiempo de alarma
    timer_hw->alarm[ALARM_NUM] = next_alarm; //programa la primera alarma

    while (true) {
        tight_loop_contents(); //mantiene el programa principal activo
    }
    return 0;
}