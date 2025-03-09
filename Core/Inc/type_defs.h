/*
 * type_defs.h
 *
 *  Created on: Mar 8, 2025
 *      Author: rodri
 */

#ifndef INC_TYPE_DEFS_H_
#define INC_TYPE_DEFS_H_

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef enum { FALSE = 0, TRUE = 1 } Bool;

typedef enum {
	STATE_CHECKING_COM,
	STATE_NO_ECHO,
	STATE_CWQAP,
	STATE_CIPSERVER_CLOSE,
    STATE_SETTING_CWMODE,
	STATE_SETTING_CIPMUX,
	STATE_CREATE_OWN_WIFI,
	STATE_CIPSERVERMAXCONN,
	STATE_CREATE_SERVER,
	STATE_SERVER_ACTIVE,
	STATE_CLIENT_CONNECTED
}Conection_State_t;

typedef enum{
	TRYING_TO_CONNECT,
	NO_TASK,
	READ_BME280,
	CIPSEND_TASK,
	SAMPLE_SENDING
}task_t;

typedef enum {
	OK,
	CONNECT,
	BUSY,
	ERR,
	AT,
	EMPTY,
	SEND_FROM_PC,
	CLOSED,
	CIPSEND_READY,
	UNKNOWN,
	READ_BME280_response,
	FAIL,
	CIPSTATE,
	CLOSE_FROM_PC,
	SEND_OK,
	START_SENDING_SAMPLES,
	STOP_SENDING_SAMPLES
}response_t; // Don't use ERROR the stm already uses it

#endif /* INC_TYPE_DEFS_H_ */
