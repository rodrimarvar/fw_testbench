/*
 * uart_comm.c
 *
 *  Created on: Mar 8, 2025
 *      Author: rodri
 */
#include <uart_reception.h>

volatile uint16_t head = 0;    // Posición de inicio antes de recibir datos
volatile uint16_t overflow_start = 0;  // Posición del buffer antes del desbordamiento

volatile char rx_buffer[BUFFER_SIZE]; //buffer unico de recepcion
char tx_buffer[BUFFER_SIZE]; //buffer de tranmisión que al menos para la conexion es unico para la transmitir datos
char message_buffer[BUFFER_SIZE]; // Buffer para almacenar un mensaje completo

volatile Bool flag_message_copied = 0;

void copy_message(uint16_t start, uint16_t end, Bool overflow) {
    uint16_t index = 0;
    // Copiar datos desde el buffer circular al `message_buffer`
    while (start != end) {
        message_buffer[index++] = rx_buffer[start];
        start = (start + 1) % BUFFER_SIZE;
        if (index >= sizeof(message_buffer) - 1) break; // Prevenir desbordamiento del buffer temporal
    }

    if(overflow == 0){
    	message_buffer[index] = '\0';  // Finalizar la cadena
    }
}


