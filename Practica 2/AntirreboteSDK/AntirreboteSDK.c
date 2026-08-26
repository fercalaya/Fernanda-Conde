#include <stdio.h>

#include "pico/stdlib.h"

#define BTN_PIN 16 //define el pin del gpio al boton

#define DEBOUNCE_DELAY_MS 30 //tiempo de espera para el antirrebote

typedef enum {

    BOTON_OFF, //estado 0: boton en reposo

    PRESIONANDO, //estado 1: espera la confirmacion del antirrebote

    BOTON_ON, //estado 2: boton confirmado y presionado

    SOLTANDO //estado 3: espera la confirmacion de soltar

} EstadoFSM;


int main() {

    stdio_init_all(); //inicia la comunicacion serial

    const uint LED_PIN = PICO_DEFAULT_LED_PIN; //define el led de la raspberry

    gpio_init(LED_PIN); //inicia el pin del led

    gpio_set_dir(LED_PIN, GPIO_OUT); //configura el led como salida

    gpio_put(LED_PIN, 0); //inicia el led apagado

    gpio_init(BTN_PIN); //inicia el pin del boton

    gpio_set_dir(BTN_PIN, GPIO_IN); //configura el boton como entrada

    gpio_pull_up(BTN_PIN); //activa resistencia pull up

    EstadoFSM estado = BOTON_OFF; //inicia la maquina en reposo

    while (true) {

        bool boton = !gpio_get(BTN_PIN); //lee el boton, 1 cuando se presiona

        switch (estado) {

            case BOTON_OFF:

                if (boton) {

                    estado = PRESIONANDO; //detecta posible presion

                }

                break;


            case PRESIONANDO:

                sleep_ms(DEBOUNCE_DELAY_MS); //espera para eliminar rebote

                if (!gpio_get(BTN_PIN)) {

                    gpio_put(LED_PIN, !gpio_get(LED_PIN)); //cambia el estado del led

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

                if (gpio_get(BTN_PIN)) {
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