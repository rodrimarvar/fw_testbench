/*
 * comm_state_machine.h
 *
 *  Created on: Mar 8, 2025
 *      Author: rodri
 */

#ifndef INC_COMM_STATE_MACHINE_H_
#define INC_COMM_STATE_MACHINE_H_

#include <type_defs.h>

typedef struct {
        const Conection_State_t state;
        const response_t *response;
        int num_responses;
        const char command[50];
        const Conection_State_t *next_state;
} com_state_wifi_card;

void update_buffer(char *buffer, size_t buffer_size, const char *new_content);
Bool handle_wifi_card_state(response_t response, com_state_wifi_card **current_wifi_com_status);

#endif /* INC_COMM_STATE_MACHINE_H_ */
