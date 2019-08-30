#include "dsched.h"
#include <stdio.h>
#include <stdlib.h>

void minute_tick(uint32_t hour, uint32_t minute)
{
    DSched_Status_TypeDef retval;
    uint32_t data;

    retval = DSched_Exec(hour, minute, &data);
    if (retval == DSCHED_STATUS_SUCCESS)
        printf("%02d:%02d - 0x%08x\n", hour, minute, data);
    else
        printf("%02d:%02d\n", hour, minute);
}

int main(void)
{
    printf("Hello from main()!\n");
    DSched_Init();

    DSched_EntryData_TypeDef data;

    data = 0xB00B0002;
    DSched_AddEntry(1, 5, &data);
    data = 0xB00B0003;
    DSched_AddEntry(1, 7, &data);
    data = 0xB00B0001;
    DSched_AddEntry(0, 34, &data);
    data = 0xB00B0005;
    DSched_AddEntry(18, 7, &data);
    data = 0xB00B0006;
    DSched_AddEntry(19, 50, &data);
    data = 0xB00B0004;
    DSched_AddEntry(15, 50, &data);
    data = 0xB00B0007;
    DSched_AddEntry(15, 51, &data);
    data = 0xB00B0008;
    DSched_AddEntry(15, 52, &data);
    data = 0xB00B0018;
    DSched_AddEntry(3, 52, &data);
    data = 0xB00B0019;
    DSched_AddEntry(16, 48, &data);
    data = 0xB00B0028;
    DSched_AddEntry(16, 47, &data);
    data = 0xB00B0038;
    DSched_AddEntry(16, 47, &data);
    data = 0xB00B0048;
    DSched_AddEntry(20, 25, &data);

    DSched_RemoveEntry(1, 5);
    DSched_RemoveEntry(19, 50);

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            minute_tick(h, m);
        }
    }

    printf("\nSchedule:\n");
    DSched_Entry_TypeDef entries[DSCHED_ENTRIES_MAX];
    uint32_t total_num;
    DSched_GetSchedule(&entries[0], &total_num);
    if (total_num == 0)
        printf("No entries!\n");
    else {
        for (uint32_t i = 0; i < total_num; i++) {
            printf("%02d:%02d - 0x%08x\n", entries[i].hour, entries[i].minute, entries[i].data);
        }
    }

    return 0;
}
