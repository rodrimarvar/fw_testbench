/*
 * data_buffer.h
 *
 *  Created on: Feb 18, 2025
 *      Author: lcastedo
 */

#ifndef DATA_BUFFER_H_INCLUDED
#define DATA_BUFFER_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef DBUF_BUFFER_LENGTH
#define DBUF_BUFFER_LENGTH (224) /**< Size of buffer in records */
#endif
#ifndef DBUF_BUNDLE_LENGTH
#define DBUF_BUNDLE_LENGTH (4) /**< Amount of records in a bundle */
#endif
/**
 * A structure to represent a measurement set
 */
struct DataRecord {
  uint32_t timeStamp; /**< sample time stamp */
  float samples[6];   /**< sensor measurements */
};

/** Bundle size in bytes */
#define DBUF_BUNDLE_SIZE (sizeof(struct DataRecord) * DBUF_BUNDLE_LENGTH)

bool dbuf_bundle_available(void);
void dbuf_clear(void);
const struct DataRecord *dbuf_current_rd_slot(void);
struct DataRecord *dbuf_current_wr_slot(void);
const struct DataRecord *dbuf_pop_record_bundle(void);
struct DataRecord *dbuf_push_record(void);
size_t dbuf_record_count(void);

#endif /* DATA_BUFFER_H_INCLUDED */
