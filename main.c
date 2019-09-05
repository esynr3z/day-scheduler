#include "dsched.h"
#include <stdio.h>
#include <stdlib.h>

void minute_tick(uint32_t hour, uint32_t minute)
{
    dsched_status_t retval;
    uint32_t data;

    retval = dsched_exec(hour, minute, &data);
    if (retval == DSCHED_STATUS_SUCCESS)
        printf("%02d:%02d - 0x%08x\n", hour, minute, data);
    else
        printf("%02d:%02d\n", hour, minute);
}

int main(void)
{
    printf("Hello from main()!\n");
    dsched_init();

    dsched_entry_data_t data;

    data = 0xB00B0002;
    dsched_add_entry(1, 5, &data);
    data = 0xB00B0003;
    dsched_add_entry(1, 7, &data);
    data = 0xB00B0001;
    dsched_add_entry(0, 34, &data);
    data = 0xB00B0005;
    dsched_add_entry(18, 7, &data);
    data = 0xB00B0006;
    dsched_add_entry(19, 50, &data);
    data = 0xB00B0004;
    dsched_add_entry(15, 50, &data);
    data = 0xB00B0007;
    dsched_add_entry(15, 51, &data);
    data = 0xB00B0008;
    dsched_add_entry(15, 52, &data);
    data = 0xB00B0018;
    dsched_add_entry(3, 52, &data);
    data = 0xB00B0019;
    dsched_add_entry(16, 48, &data);
    data = 0xB00B0028;
    dsched_add_entry(16, 47, &data);
    data = 0xB00B0038;
    dsched_add_entry(16, 47, &data);
    data = 0xB00B0048;
    dsched_add_entry(20, 25, &data);

    dsched_remove_entry(1, 5);
    dsched_remove_entry(19, 50);

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            minute_tick(h, m);
        }
    }

    printf("\nSchedule:\n");
    dsched_entry_t entries[DSCHED_ENTRIES_MAX];
    uint32_t total_num;
    dsched_get_schedule(&entries[0], &total_num);
    if (total_num == 0)
        printf("No entries!\n");
    else {
        for (uint32_t i = 0; i < total_num; i++) {
            printf("%02d:%02d - 0x%08x\n", entries[i].hour, entries[i].minute, entries[i].data);
        }
    }

    return 0;
}
