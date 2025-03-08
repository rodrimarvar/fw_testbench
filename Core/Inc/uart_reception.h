/*
 * uart_comm.h
 *
 *  Created on: Mar 8, 2025
 *      Author: rodri
 */

#ifndef INC_UART_COMM_H_
#define INC_UART_COMM_H_

#include <type_defs.h>

#define BUFFER_SIZE 128

void copy_message(uint16_t start, uint16_t end, Bool overflow);

#endif /* INC_UART_COMM_H_ */
