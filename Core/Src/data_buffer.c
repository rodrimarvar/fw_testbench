/*
 * data_buffer.c
 *
 *  Created on: Feb 18, 2025
 *      Author: lcastedo
 */

#include <data_buffer.h>
#include <assert.h>

static struct DataRecord dbuf_storage[DBUF_BUFFER_LENGTH];
static struct DataRecord *dbuf_wr_ptr = dbuf_storage;
static struct DataRecord *dbuf_rd_ptr = dbuf_storage;
bool dbuf_empty_flag = true;

static const struct DataRecord *dbuf_storage_boundary =
    dbuf_storage + DBUF_BUFFER_LENGTH;

/**
 * Check if there is enough records for a bundle in buffer.
 *
 * This function check if there are BUNDLE_SIZE or more records in buffer.

 * @return Returns true if enough records for bundle.
 */
bool dbuf_bundle_available(void) {
  return dbuf_record_count() >= DBUF_BUNDLE_LENGTH;
}

/**
 * Clear buffer.
 *
 * This function reset buffer and render it empty.
 */
void dbuf_clear(void) {
  dbuf_wr_ptr = dbuf_rd_ptr = dbuf_storage;
  dbuf_empty_flag = true;
}

/**
 * Get current read position.
 *
 * This function returns a pointer to current read slot in buffer.
 *
 * @return Non-writable pointer to current read slot if bundle
 *         available, NULL otherwise.
 */
const struct DataRecord *dbuf_current_rd_slot(void) {
  return dbuf_bundle_available() ? dbuf_rd_ptr : NULL;
}

/**
 * Get current write position.
 *
 * This function returns a pointer to current write slot in buffer.

 * @return Writeable pointer to current write slot if space in buffer
 *         available, NULL otherwise.
 */
struct DataRecord *dbuf_current_wr_slot(void) {
  return (dbuf_wr_ptr != dbuf_rd_ptr) || dbuf_empty_flag ? dbuf_wr_ptr : NULL;
}

/**
 * Increment read pointer by BUNDLE_SIZE positions.
 *
 * @return Next slot read pointer.
 */
const struct DataRecord *dbuf_pop_record_bundle(void) {
  static_assert(DBUF_BUFFER_LENGTH % DBUF_BUNDLE_LENGTH == 0,
                "BUFFER_SIZE must be a multiple of BUNDLE_SIZE.");
  assert(dbuf_record_count() >= DBUF_BUNDLE_LENGTH);

  dbuf_rd_ptr += DBUF_BUNDLE_LENGTH;
  if (dbuf_rd_ptr >= dbuf_storage_boundary)
    dbuf_rd_ptr = dbuf_storage;
  if (dbuf_rd_ptr == dbuf_wr_ptr)
    dbuf_empty_flag = true;
  return dbuf_rd_ptr;
}

/**
 * Increment write pointer by one position.
 *
 * @return Next slot write pointer.
 */
struct DataRecord *dbuf_push_record(void) {
  if (++dbuf_wr_ptr >= dbuf_storage_boundary)
    dbuf_wr_ptr = dbuf_storage;
  dbuf_empty_flag = false;
  return dbuf_wr_ptr;
}

/**
 * Return the number of records stored in buffer.
 *
 * @return Number of records in buffer.
 */
size_t dbuf_record_count(void) {
  if (dbuf_empty_flag)
    return 0;

  ptrdiff_t dif = dbuf_wr_ptr - dbuf_rd_ptr;
  return (size_t)(dif > 0 ? dif : dif + DBUF_BUFFER_LENGTH);
}
