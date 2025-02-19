/*
 * cobs.h
 *
 *  Created on: Feb 19, 2025
 *      Author: rodri
 */

#ifndef INC_COBS_H_
#define INC_COBS_H_

#include <stddef.h>
#include <stdint.h>

size_t cobsEncode(const void *data, size_t length, uint8_t *buffer);
size_t cobsDecode(const uint8_t *buffer, size_t length, void *data);


#endif /* INC_COBS_H_ */
