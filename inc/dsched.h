#ifndef __DSCHED_H
#define __DSCHED_H

//-- Includes ------------------------------------------------------------------
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//-- Exported macro ------------------------------------------------------------
#define DSCHED_ENTRIES_MAX 64

//-- Exported types ---------------------------------------------------------
typedef uint32_t dsched_entry_data_t;

typedef struct {
    uint32_t minute;
    uint32_t hour;
    dsched_entry_data_t data;
} dsched_entry_t;

typedef enum {
    DSCHED_STATUS_SUCCESS,
    DSCHED_STATUS_OVERFLOW,
    DSCHED_STATUS_UNDERFLOW,
    DSCHED_STATUS_FAIL,
    DSCHED_STATUS_EMPTY
} dsched_status_t;

//-- Exported functions --------------------------------------------------------
dsched_status_t dsched_init(void);
dsched_status_t dsched_exec(uint32_t hour, uint32_t minute, dsched_entry_data_t* data);
dsched_status_t dsched_add_entry(uint32_t hour, uint32_t minute, dsched_entry_data_t* data);
dsched_status_t dsched_remove_entry(uint32_t hour, uint32_t minute);
dsched_status_t dsched_get_schedule(dsched_entry_t* entries, uint32_t* total_num);

#endif // __DSCHED_H
