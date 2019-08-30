//-- Includes ------------------------------------------------------------------
#include "dsched.h"

//-- Private types -----------------------------------------------------------
typedef struct Entry {
    bool empty;                    // if entry is present, or not and can be overwritten
    uint32_t minute;               // minute which entry is assigned
    uint32_t hour;                 // hour which entry is assigned
    DSched_EntryData_TypeDef data; // data for a entry
    struct Entry* next;            // next entry
    struct Entry* prev;            // prev entry
} Entry_TypeDef;

//-- Private functions prototypes ----------------------------------------------
static uint32_t time_to_dminute(uint32_t hour, uint32_t minute);
static DSched_Status_TypeDef get_nearest_entry(uint32_t dminute, Entry_TypeDef** entry);
static int compare_entries_time(const void* a, const void* b);

//-- Private variables ---------------------------------------------------------
static struct
{
    uint32_t entries_total;
    Entry_TypeDef entry_slot[DSCHED_ENTRIES_MAX];
    Entry_TypeDef* next_entry;
} schedule;

//-- Exported functions --------------------------------------------------------
DSched_Status_TypeDef DSched_Init(void)
{
    schedule.next_entry = NULL;
    schedule.entries_total = 0;
    memset(schedule.entry_slot, 0, sizeof (schedule.entry_slot));
    for (uint32_t i = 0; i < DSCHED_ENTRIES_MAX; i++)
        schedule.entry_slot[i].empty = true;

    return DSCHED_STATUS_SUCCESS;
}

DSched_Status_TypeDef DSched_Exec(uint32_t hour, uint32_t minute, DSched_EntryData_TypeDef* data)
{
    uint32_t dminute = time_to_dminute(hour, minute);

    if (schedule.entries_total) {
        Entry_TypeDef* nearest_entry = NULL;
        if (!schedule.next_entry) {
            // on the first call we should select next entry from the nearest to current time
            // and return it's data
            DSched_Status_TypeDef retval;
            retval = get_nearest_entry(dminute, &nearest_entry);
            if (retval != DSCHED_STATUS_SUCCESS)
                return retval;
            if ((int)(dminute - time_to_dminute(nearest_entry->hour, nearest_entry->minute)) >= 0) {
                memcpy(data, &nearest_entry->data, sizeof(nearest_entry->data));
                schedule.next_entry = nearest_entry->next;
            } else {
                memcpy(data, &nearest_entry->prev->data, sizeof(nearest_entry->prev->data));
                schedule.next_entry = nearest_entry;
            }
            return DSCHED_STATUS_SUCCESS;
        } else if (dminute == time_to_dminute(schedule.next_entry->hour, schedule.next_entry->minute)) {
            *data = schedule.next_entry->data;
            schedule.next_entry = schedule.next_entry->next;
            return DSCHED_STATUS_SUCCESS;
        }
    }

    return DSCHED_STATUS_EMPTY;
}

DSched_Status_TypeDef DSched_AddEntry(uint32_t hour, uint32_t minute, DSched_EntryData_TypeDef* data)
{
    uint32_t dminute = time_to_dminute(hour, minute);

    if (schedule.entries_total == DSCHED_ENTRIES_MAX)
        return DSCHED_STATUS_OVERFLOW;
    else if (schedule.entries_total == 0) {
        // special case of the first entry
        memcpy(&schedule.entry_slot[0].data, data, sizeof(schedule.entry_slot[0].data));
        schedule.entry_slot[0].hour = hour;
        schedule.entry_slot[0].minute = minute;
        schedule.entry_slot[0].next = &schedule.entry_slot[0];
        schedule.entry_slot[0].prev = &schedule.entry_slot[0];
        schedule.entry_slot[0].empty = false;
        schedule.entries_total++;
    } else {
        // find the nearest empty entry
        int32_t nearest_empty_n = -1;
        for (uint32_t i = 0; i < DSCHED_ENTRIES_MAX; i++) {
            if (schedule.entry_slot[i].empty) {
                nearest_empty_n = i;
                break;
            }
        }
        if (nearest_empty_n < 0)
            return DSCHED_STATUS_OVERFLOW; // if all slots are not empty - index will not be updated

        // find the nearest not empty entry by time
        Entry_TypeDef* nearest_entry = NULL;
        DSched_Status_TypeDef retval;
        retval = get_nearest_entry(dminute, &nearest_entry);
        if (retval != DSCHED_STATUS_SUCCESS)
            return retval;
        if (dminute == time_to_dminute(nearest_entry->hour, nearest_entry->minute))
            return DSCHED_STATUS_FAIL; // all entries should have different time

        // save the new entry
        if ((int)(dminute - time_to_dminute(nearest_entry->hour, nearest_entry->minute)) > 0) {
            schedule.entry_slot[nearest_empty_n].next = nearest_entry->next;
            schedule.entry_slot[nearest_empty_n].prev = nearest_entry;
            nearest_entry->next->prev = &schedule.entry_slot[nearest_empty_n];
            nearest_entry->next = &schedule.entry_slot[nearest_empty_n];
        } else {
            schedule.entry_slot[nearest_empty_n].next = nearest_entry;
            schedule.entry_slot[nearest_empty_n].prev = nearest_entry->prev;
            nearest_entry->prev->next = &schedule.entry_slot[nearest_empty_n];
            nearest_entry->prev = &schedule.entry_slot[nearest_empty_n];
        }
        memcpy(&schedule.entry_slot[nearest_empty_n].data, data, sizeof(schedule.entry_slot[nearest_empty_n].data));
        schedule.entry_slot[nearest_empty_n].hour = hour;
        schedule.entry_slot[nearest_empty_n].minute = minute;
        schedule.entry_slot[nearest_empty_n].empty = false;
        schedule.entries_total++;
    }

    return DSCHED_STATUS_SUCCESS;
}

DSched_Status_TypeDef DSched_RemoveEntry(uint32_t hour, uint32_t minute)
{
    uint32_t dminute = time_to_dminute(hour, minute);

    if (schedule.entries_total == 0)
        return DSCHED_STATUS_UNDERFLOW;
    else {
        // find needed entry
        int32_t del_entry_n = -1;
        for (uint32_t i = 0; i < DSCHED_ENTRIES_MAX; i++) {
            if (!schedule.entry_slot[i].empty &&
                (time_to_dminute(schedule.entry_slot[i].hour, schedule.entry_slot[i].minute) == dminute)) {
                del_entry_n = i;
                break;
            }
        }
        if (del_entry_n < 0)
            return DSCHED_STATUS_FAIL;

        // delete the entry
        schedule.entry_slot[del_entry_n].prev->next = schedule.entry_slot[del_entry_n].next;
        schedule.entry_slot[del_entry_n].next->prev = schedule.entry_slot[del_entry_n].prev;
        schedule.entry_slot[del_entry_n].empty = true;
        schedule.entries_total--;
    }

    return DSCHED_STATUS_SUCCESS;
}

DSched_Status_TypeDef DSched_GetSchedule(DSched_Entry_TypeDef* entries, uint32_t* total_num)
{
    DSched_Entry_TypeDef tmp_entries[DSCHED_ENTRIES_MAX];
    uint32_t tmp_total_num = 0;

    for (uint32_t i = 0; i < DSCHED_ENTRIES_MAX; i++) {
        if (!schedule.entry_slot[i].empty) {
            tmp_entries[i].hour = schedule.entry_slot[i].hour;
            tmp_entries[i].minute = schedule.entry_slot[i].minute;
            memcpy(&tmp_entries[i].data, &schedule.entry_slot[i].data, sizeof(tmp_entries[i].data));
            tmp_total_num++;
        } else {
            tmp_entries[i].hour = 99;
            tmp_entries[i].minute = 99;
        }
    }
    *total_num = tmp_total_num;

    if (tmp_total_num == 0)
        return DSCHED_STATUS_FAIL;

    qsort(tmp_entries, DSCHED_ENTRIES_MAX, sizeof(tmp_entries[0]), compare_entries_time);
    memcpy(entries, &tmp_entries, sizeof(tmp_entries[0]) * tmp_total_num);

    return DSCHED_STATUS_SUCCESS;
}

//-- Private functions ---------------------------------------------------------
static uint32_t time_to_dminute(uint32_t hour, uint32_t minute)
{
    return hour * 60 + minute;
}

static DSched_Status_TypeDef get_nearest_entry(uint32_t dminute, Entry_TypeDef** entry)
{
    Entry_TypeDef* nearest_entry = NULL;
    for (uint32_t i = 0; i < DSCHED_ENTRIES_MAX; i++) {
        if (!schedule.entry_slot[i].empty) {
            if (!nearest_entry)
                nearest_entry = &schedule.entry_slot[i]; // we should take the first not empty entry as the nearest
            if (abs((int)(dminute - time_to_dminute(schedule.entry_slot[i].hour, schedule.entry_slot[i].minute))) <=
                abs((int)(dminute - time_to_dminute(nearest_entry->hour, nearest_entry->minute)))) {
                nearest_entry = &schedule.entry_slot[i];
            }
        }
    }
    if (!nearest_entry)
        return DSCHED_STATUS_OVERFLOW; // if all slots are empty - pointer will not be updated

    *entry = nearest_entry;
    return DSCHED_STATUS_SUCCESS;
}

static int compare_entries_time(const void* a, const void* b)
{
    const DSched_Entry_TypeDef* arg1 = a;
    const DSched_Entry_TypeDef* arg2 = b;

    return time_to_dminute(arg1->hour, arg1->minute) - time_to_dminute(arg2->hour, arg2->minute);
}
