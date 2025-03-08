/*
 * task_handling.h
 *
 *  Created on: Mar 8, 2025
 *      Author: rodri
 */

#ifndef INC_TASK_HANDLING_H_
#define INC_TASK_HANDLING_H_

#include <type_defs.h>
#include <comm_state_machine.h>

typedef struct {
        const char *keyword;
        response_t response;
        task_t task;
} KeywordResponse;

KeywordResponse match_respones(char* line, Bool connected);
void task_handling(KeywordResponse keyword);
void from_message_to_tasks(char* message);

#endif /* INC_TASK_HANDLING_H_ */
