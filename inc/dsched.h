#ifndef __DSCHED_H
#define __DSCHED_H

//-- Includes ------------------------------------------------------------------
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//-- Exported macro ------------------------------------------------------------
#define DSCHED_ENTRIES_MAX 64

//-- Exported types ---------------------------------------------------------
typedef uint32_t DSched_EntryData_TypeDef;

typedef struct {
    uint32_t minute;
    uint32_t hour;
    DSched_EntryData_TypeDef data;
} DSched_Entry_TypeDef;

typedef enum {
    DSCHED_STATUS_SUCCESS,
    DSCHED_STATUS_OVERFLOW,
    DSCHED_STATUS_UNDERFLOW,
    DSCHED_STATUS_FAIL,
    DSCHED_STATUS_EMPTY
} DSched_Status_TypeDef;

//-- Exported functions --------------------------------------------------------
DSched_Status_TypeDef DSched_Init(void);
DSched_Status_TypeDef DSched_Exec(uint32_t hour, uint32_t minute, DSched_EntryData_TypeDef* data);
DSched_Status_TypeDef DSched_AddEntry(uint32_t hour, uint32_t minute, DSched_EntryData_TypeDef* data);
DSched_Status_TypeDef DSched_RemoveEntry(uint32_t hour, uint32_t minute);
DSched_Status_TypeDef DSched_GetSchedule(DSched_Entry_TypeDef *entries, uint32_t* total_num);

#endif // __DSCHED_H
