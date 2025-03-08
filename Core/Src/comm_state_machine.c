/*
 * comm_state_machine.c
 *
 *  Created on: Mar 8, 2025
 *      Author: rodri
 */

#include <comm_state_machine.h>

com_state_wifi_card com_wifi_card_values[] = {
        {STATE_CHECKING_COM, (response_t[]){OK}, 1,"AT+CIPMODE=0\r\n",(Conection_State_t[]){STATE_NO_ECHO}},
		{STATE_NO_ECHO, (response_t[]){OK}, 1,"ATE1\r\n",(Conection_State_t[]){STATE_CWQAP}},
		{STATE_CWQAP, (response_t[]){OK}, 1,"AT+CWQAP\r\n",(Conection_State_t[]){STATE_CIPSERVER_CLOSE}},
		{STATE_CIPSERVER_CLOSE, (response_t[]){OK, ERR}, 2,"AT+CIPSERVER=0\r\n",(Conection_State_t[]){STATE_SETTING_CWMODE, STATE_SETTING_CWMODE}},
        {STATE_SETTING_CWMODE, (response_t[]){OK}, 1,"AT+CWMODE=2\r\n",(Conection_State_t[]){STATE_SETTING_CIPMUX}},
		{STATE_SETTING_CIPMUX, (response_t[]){OK}, 1,"AT+CIPMUX=1\r\n",(Conection_State_t[]){STATE_CREATE_OWN_WIFI}},
		{STATE_CREATE_OWN_WIFI, (response_t[]){OK}, 1,"AT+CWSAP=\"MI PUNTO\",\"12345678\",3,0\r\n",(Conection_State_t[]){STATE_CREATE_SERVER}},
		{STATE_CREATE_SERVER, (response_t[]){OK}, 1,"AT+CIPSERVER=1,8000\r\n",(Conection_State_t[]){STATE_SERVER_ACTIVE}},
		{STATE_SERVER_ACTIVE, (response_t[]){CONNECT, FAIL}, 2,"AT\r\n",(Conection_State_t[]){STATE_SERVER_ACTIVE, STATE_CIPSERVER_CLOSE}},
		//{STATE_CLIENT_CONNECTED, (response_t[]){FAIL}, 1,"AT\r\n",(Conection_State_t[]){STATE_CIPSERVER_CLOSE}},

};

com_state_wifi_card* current_wifi_com_status=&com_wifi_card_values[0];
Bool client_connected = 0;

Bool handle_wifi_card_state(response_t response, com_state_wifi_card **current_wifi_com_status){
	for(size_t i = 0;i < (*current_wifi_com_status)->num_responses;i++){
		Bool match = 0;
		if((*current_wifi_com_status)->response[i] == response){
			for(size_t k = 0; k < (sizeof(com_wifi_card_values)/sizeof(com_state_wifi_card));k++){
				if((*current_wifi_com_status)->next_state[i]== com_wifi_card_values[k].state){
					match=1;
					(*current_wifi_com_status) = &com_wifi_card_values[k];
					break;
				}
			}
		}
		if(match){
			break;
		}
	}
	if((*current_wifi_com_status)->state == STATE_SERVER_ACTIVE){
		return 1;
	}
	return 0;
}
