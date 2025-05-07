/*
 * task_handling.c
 *
 *  Created on: Mar 8, 2025
 *      Author: rodri
 */

#include <task_handling.h>

extern Bool client_connected;
extern com_state_wifi_card* current_wifi_com_status;
extern com_state_wifi_card com_wifi_card_values[];
extern uint32_t time_init;

Bool flag_read_bme280 = 0, flag_cipsend = 0, flag_sample_sending = 0;

KeywordResponse keywords_for_connection[] = {
        {"CONNECT", CONNECT, TRYING_TO_CONNECT},
        {"OK", OK, TRYING_TO_CONNECT},
        {"ERROR", ERR, TRYING_TO_CONNECT},
        {"busy", BUSY, NO_TASK},
        {"AT", AT, NO_TASK},
		{">", CIPSEND_READY, NO_TASK},
		{"CLOSED", CLOSED, TRYING_TO_CONNECT},
		{"READ_BME280",READ_BME280_response, NO_TASK},
		{"FAIL", FAIL, TRYING_TO_CONNECT},
        {"CIPSTATE", CIPSTATE, NO_TASK},
        {"CLOSE_FROM_PC", CLOSE_FROM_PC, NO_TASK},
		{"SEND", SEND_OK, NO_TASK}
};

KeywordResponse keywords_when_connected[] = {
        //{"CONNECT", CONNECT, NO_TASK},
        {"OK", OK, NO_TASK},
        {"ERROR", ERR, CIPSEND_TASK},
        {"busy", BUSY, NO_TASK},
        //{"AT", AT, NO_TASK},
		{">", CIPSEND_READY, CIPSEND_TASK},
		{"CLOSED", CLOSED, TRYING_TO_CONNECT},
		{"READ_BME280",READ_BME280_response, READ_BME280},
		{"FAIL", FAIL, TRYING_TO_CONNECT},
        //{"CIPSTATE", CIPSTATE, NO_TASK},
        //{"CLOSE_FROM_PC", CLOSE_FROM_PC, NO_TASK},
		{"SEND", SEND_OK, CIPSEND_TASK},
        {"START", START_SENDING_SAMPLES, SAMPLE_SENDING},
		{"STOP", STOP_SENDING_SAMPLES, SAMPLE_SENDING},
		{"valid", INVALID_LINK, SAMPLE_SENDING},
		{"link", INVALID_LINK, SAMPLE_SENDING}
};

KeywordResponse default_keyword = {"", EMPTY, NO_TASK};

KeywordResponse match_responses(char* line, Bool connected) {
    KeywordResponse *keywords;  // Se usa un solo puntero, no doble puntero
    int num_keywords;

    if (connected) {
        keywords = keywords_when_connected;
        num_keywords = sizeof(keywords_when_connected) / sizeof(keywords_when_connected[0]);
    } else {
        keywords = keywords_for_connection;
        num_keywords = sizeof(keywords_for_connection) / sizeof(keywords_for_connection[0]);
    }

    for (int k = 0; k < num_keywords; k++) {
        if (strstr(line, keywords[k].keyword) != NULL) {
            return keywords[k];  // Devolver la coincidencia encontrada
        }
    }

    return default_keyword;  // Devolver un valor por defecto si no hay coincidencias
}


void task_handling(KeywordResponse keyword){
	switch(keyword.task){
		case TRYING_TO_CONNECT:
			flag_cipsend = 0;
			client_connected = handle_wifi_card_state(keyword.response, &current_wifi_com_status);
			break;
		case READ_BME280:
			flag_read_bme280 = 1;
			break;
		case CIPSEND_TASK:
			if(flag_cipsend == 0){
				flag_cipsend = 1;
			}
			break;
		case SAMPLE_SENDING:
			if(keyword.response == START_SENDING_SAMPLES){
				flag_sample_sending = 1;
				time_init = HAL_GetTick();
			}
			else{
				flag_sample_sending = 0;
			}
			break;
		case NO_TASK:
			break;
		default:
			break;
	}
}

void from_message_to_tasks(char* message) {
    char *line = strtok(message, "\n");  // Dividir mensaje en líneas
    KeywordResponse keywordresponse;

    while (line != NULL) {
        keywordresponse = match_responses(line, client_connected);  // Convertir en respuesta
        task_handling(keywordresponse);
        line = strtok(NULL, "\n");  // Obtener siguiente línea
    }
}
