#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/structs/sio.h"

#define BTN_PIN 16 //define el pin del gpio al boton
#define LED_PIN PICO_DEFAULT_LED_PIN //define el pin del led
#define DEBOUNCE_DELAY_MS 30 //tiempo de espera para el antirrebote

typedef enum {
    BOTON_OFF, //estado 0: boton en reposo
    PRESIONANDO, //estado 1: espera la confirmacion del antirrebote
    BOTON_ON, //estado 2: boton confirmado y presionado
    SOLTANDO //estado 3: espera la confirmacion de soltar
} EstadoFSM;

int main() {
    stdio_init_all(); //inicia la comunicacion serial
    gpio_init(BTN_PIN); //inicia el pin del boton
    gpio_set_dir(BTN_PIN, GPIO_IN); //configura el boton como entrada
    gpio_pull_up(BTN_PIN); //activa resistencia pull up
    gpio_init(LED_PIN); //inicia el pin del led
    gpio_set_dir(LED_PIN, GPIO_OUT); //configura el led como salida
    sio_hw->gpio_clr = 1u << LED_PIN; //inicia el led apagado
    EstadoFSM estado = BOTON_OFF; //inicia la maquina en reposo

    while (true) {
        bool boton = !(sio_hw->gpio_in & (1u << BTN_PIN)); //lee el boton
        switch (estado) {
            case BOTON_OFF:
                if (boton) {
                    estado = PRESIONANDO; //detecta posible presion
                }
                break;
            case PRESIONANDO:
                sleep_ms(DEBOUNCE_DELAY_MS); //espera para eliminar rebote
                if (!(sio_hw->gpio_in & (1u << BTN_PIN))) {
                    sio_hw->gpio_togl = 1u << LED_PIN; //cambia el estado del led
                    estado = BOTON_ON; //confirma la presion
                }
                else {
                    estado = BOTON_OFF; //descarta el rebote
                }
                break;
            case BOTON_ON:
                if (!boton) {
                    estado = SOLTANDO; //detecta que se solto
                }
                break;
            case SOLTANDO:
                sleep_ms(DEBOUNCE_DELAY_MS); //espera para eliminar rebote
                if (sio_hw->gpio_in & (1u << BTN_PIN)) {
                    estado = BOTON_OFF; //confirma que se solto
                }
                else {
                    estado = BOTON_ON; //continua presionado
                }
                break;
        }
        sleep_ms(1); //pequena espera del ciclo
    }
    return 0;
}
