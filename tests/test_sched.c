#include "dsched.h"
#include <check.h>
#include <stdlib.h>

START_TEST(test_add)
{
    dsched_status_t retval;
    dsched_entry_data_t data;
    dsched_entry_data_t stimuli[24][60] = { { 0 } };

    dsched_init();
    stimuli[0][0] = 0xB00A0101;
    stimuli[1][28] = 0xB00B0101;
    stimuli[5][20] = 0xB00B0012;
    stimuli[6][35] = 0xB01B0012;
    stimuli[10][50] = 0xB01B0112;
    stimuli[15][18] = 0xB10A0101;
    stimuli[19][33] = 0xA10A0101;
    stimuli[22][37] = 0xA10A1101;

    retval = dsched_add_entry(1, 28, &stimuli[1][28]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(5, 20, &stimuli[5][20]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(0, 0, &stimuli[0][0]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(19, 33, &stimuli[19][33]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(6, 35, &stimuli[6][35]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(10, 50, &stimuli[10][50]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(22, 37, &stimuli[22][37]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(15, 18, &stimuli[15][18]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            retval = dsched_exec(h, m, &data);
            if (retval == DSCHED_STATUS_SUCCESS) {
                ck_assert_msg((data == stimuli[h][m]), "Got 0x%08x, expected 0x%08x at %02d:%02d", data, stimuli[h][m], h, m);
            } else if (retval == DSCHED_STATUS_EMPTY) {
                ck_assert_msg((stimuli[h][m] == 0), "Got empty status of non-empty stimuli at %02d:%02d", h, m);
            } else {
                ck_assert_msg(0, "Got error at %02d:%02d", h, m);
            }
        }
    }
}
END_TEST

START_TEST(test_add_once)
{
    dsched_status_t retval;
    dsched_entry_data_t data;
    dsched_entry_data_t stimuli[24][60] = { { 0 } };

    dsched_init();

    stimuli[2][5] = 0xB00B0002;
    stimuli[0][0] = stimuli[2][5];
    retval = dsched_add_entry(2, 5, &stimuli[2][5]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            retval = dsched_exec(h, m, &data);
            if (retval == DSCHED_STATUS_SUCCESS) {
                ck_assert_msg((data == stimuli[h][m]), "Got 0x%08x, expected 0x%08x at %02d:%02d", data, stimuli[h][m], h, m);
            } else if (retval == DSCHED_STATUS_EMPTY) {
                ck_assert_msg((stimuli[h][m] == 0), "Got empty status of non-empty stimuli at %02d:%02d", h, m);
            } else {
                ck_assert_msg(0, "Got error at %02d:%02d", h, m);
            }
        }
    }
}
END_TEST

START_TEST(test_add_one_before)
{
    dsched_status_t retval;
    dsched_entry_data_t data;
    dsched_entry_data_t stimuli[24][60] = { { 0 } };

    dsched_init();
    stimuli[1][28] = 0xB00B0011;
    stimuli[2][3] = 0xB00B0012;
    stimuli[0][0] = stimuli[2][3];
    retval = dsched_add_entry(2, 3, &stimuli[2][3]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(1, 28, &stimuli[1][28]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            retval = dsched_exec(h, m, &data);
            if (retval == DSCHED_STATUS_SUCCESS) {
                ck_assert_msg((data == stimuli[h][m]), "Got 0x%08x, expected 0x%08x at %02d:%02d", data, stimuli[h][m], h, m);
            } else if (retval == DSCHED_STATUS_EMPTY) {
                ck_assert_msg((stimuli[h][m] == 0), "Got empty status of non-empty stimuli at %02d:%02d", h, m);
            } else {
                ck_assert_msg(0, "Got error at %02d:%02d", h, m);
            }
        }
    }
}
END_TEST

START_TEST(test_add_one_after)
{
    dsched_status_t retval;
    dsched_entry_data_t data;
    dsched_entry_data_t stimuli[24][60] = { { 0 } };

    dsched_init();
    stimuli[1][28] = 0xB00B0101;
    stimuli[5][20] = 0xB00B0012;
    stimuli[0][0] = stimuli[5][20];
    retval = dsched_add_entry(1, 28, &stimuli[1][28]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(5, 20, &stimuli[5][20]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            retval = dsched_exec(h, m, &data);
            if (retval == DSCHED_STATUS_SUCCESS) {
                ck_assert_msg((data == stimuli[h][m]), "Got 0x%08x, expected 0x%08x at %02d:%02d", data, stimuli[h][m], h, m);
            } else if (retval == DSCHED_STATUS_EMPTY) {
                ck_assert_msg((stimuli[h][m] == 0), "Got empty status of non-empty stimuli at %02d:%02d", h, m);
            } else {
                ck_assert_msg(0, "Got error at %02d:%02d", h, m);
            }
        }
    }
}
END_TEST

START_TEST(test_add_overflow)
{
    dsched_status_t retval;
    dsched_entry_data_t stimuli[24][60] = { { 0 } };

    dsched_init();

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            stimuli[h][m] = (h << 16) | m;
        }
    }

    for (int i = 0; i < DSCHED_ENTRIES_MAX; i++) {
        uint32_t h = 13 + i / 60;
        uint32_t m = i % 60;
        retval = dsched_add_entry(h, m, &stimuli[h][m]);
        ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add %0d entry failed!", i);
    }

    retval = dsched_add_entry(0, 0, &stimuli[0][0]);
    ck_assert_msg(retval == DSCHED_STATUS_OVERFLOW, "Expected overflow but nothing happen!");
}
END_TEST

START_TEST(test_add_collision)
{
    dsched_status_t retval;
    dsched_entry_data_t stimuli[24][60] = { { 0 } };

    dsched_init();

    stimuli[2][5] = 0xB00B0002;
    retval = dsched_add_entry(2, 5, &stimuli[2][5]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    retval = dsched_add_entry(2, 5, &stimuli[0][0]);
    ck_assert_msg(retval == DSCHED_STATUS_FAIL, "Expect collision but nothing happen!");
}
END_TEST

START_TEST(test_add_after_remove)
{
    dsched_status_t retval;
    dsched_entry_data_t data;
    dsched_entry_data_t stimuli[24][60] = { { 0 } };

    dsched_init();
    stimuli[0][0] = 0xB00A0101;
    stimuli[1][28] = 0xB00B0101;
    stimuli[5][20] = 0xB00B0012;
    stimuli[6][35] = 0xB01B0012;
    stimuli[10][50] = 0xB01B0112;
    stimuli[15][18] = 0xB10A0101;
    stimuli[19][33] = 0xA10A0101;
    stimuli[22][37] = 0xA10A1101;

    retval = dsched_add_entry(1, 28, &stimuli[1][28]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(5, 20, &stimuli[5][20]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(0, 0, &stimuli[0][0]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(19, 33, &stimuli[19][33]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(6, 35, &stimuli[6][35]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(10, 50, &stimuli[10][50]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(22, 37, &stimuli[22][37]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(15, 18, &stimuli[15][18]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    stimuli[1][28] = 0;
    retval = dsched_remove_entry(1, 28);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Remove entry failed!");
    stimuli[5][20] = 0;
    retval = dsched_remove_entry(5, 20);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Remove entry failed!");
    stimuli[15][18] = 0;
    retval = dsched_remove_entry(15, 18);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Remove entry failed!");

    stimuli[5][20] = 0xE0016655;
    retval = dsched_add_entry(5, 20, &stimuli[5][20]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    stimuli[10][16] = 0xE0016666;
    retval = dsched_add_entry(10, 16, &stimuli[10][16]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            retval = dsched_exec(h, m, &data);
            if (retval == DSCHED_STATUS_SUCCESS) {
                ck_assert_msg((data == stimuli[h][m]), "Got 0x%08x, expected 0x%08x at %02d:%02d", data, stimuli[h][m], h, m);
            } else if (retval == DSCHED_STATUS_EMPTY) {
                ck_assert_msg((stimuli[h][m] == 0), "Got empty status of non-empty stimuli at %02d:%02d", h, m);
            } else {
                ck_assert_msg(0, "Got error at %02d:%02d", h, m);
            }
        }
    }
}
END_TEST

START_TEST(test_remove)
{
    dsched_status_t retval;
    dsched_entry_data_t data;
    dsched_entry_data_t stimuli[24][60] = { { 0 } };

    dsched_init();
    stimuli[0][0] = 0xB00A0101;
    stimuli[1][28] = 0xB00B0101;
    stimuli[5][20] = 0xB00B0012;
    stimuli[6][35] = 0xB01B0012;
    stimuli[10][50] = 0xB01B0112;
    stimuli[15][18] = 0xB10A0101;
    stimuli[19][33] = 0xA10A0101;
    stimuli[22][37] = 0xA10A1101;

    retval = dsched_add_entry(1, 28, &stimuli[1][28]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(5, 20, &stimuli[5][20]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(0, 0, &stimuli[0][0]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(19, 33, &stimuli[19][33]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(6, 35, &stimuli[6][35]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(10, 50, &stimuli[10][50]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(22, 37, &stimuli[22][37]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(15, 18, &stimuli[15][18]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    stimuli[1][28] = 0;
    retval = dsched_remove_entry(1, 28);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Remove entry failed!");
    stimuli[5][20] = 0;
    retval = dsched_remove_entry(5, 20);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Remove entry failed!");
    stimuli[15][18] = 0;
    retval = dsched_remove_entry(15, 18);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Remove entry failed!");

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            retval = dsched_exec(h, m, &data);
            if (retval == DSCHED_STATUS_SUCCESS) {
                ck_assert_msg((data == stimuli[h][m]), "Got 0x%08x, expected 0x%08x at %02d:%02d", data, stimuli[h][m], h, m);
            } else if (retval == DSCHED_STATUS_EMPTY) {
                ck_assert_msg((stimuli[h][m] == 0), "Got empty status of non-empty stimuli at %02d:%02d", h, m);
            } else {
                ck_assert_msg(0, "Got error at %02d:%02d", h, m);
            }
        }
    }
}
END_TEST

START_TEST(test_remove_underflow)
{
    dsched_status_t retval;

    dsched_init();

    retval = dsched_remove_entry(2, 5);
    ck_assert_msg(retval == DSCHED_STATUS_UNDERFLOW, "Expected underflow but nothing happpen!");
}
END_TEST

START_TEST(test_remove_nonexistent)
{
    dsched_status_t retval;
    dsched_entry_data_t stimuli[24][60] = { { 0 } };

    dsched_init();

    stimuli[2][5] = 0xB00B0002;
    retval = dsched_add_entry(2, 5, &stimuli[2][5]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    retval = dsched_remove_entry(0, 0);
    ck_assert_msg(retval == DSCHED_STATUS_FAIL, "Expected fail but nothing happpen!");
}
END_TEST

START_TEST(test_get_schedule)
{
    dsched_status_t retval;
    dsched_entry_t entries[DSCHED_ENTRIES_MAX];
    dsched_entry_data_t stimuli[24][60] = { { 0 } };
    uint32_t total_num;

    dsched_init();

    stimuli[0][0] = 0xB00A0101;
    stimuli[1][28] = 0xB00B0101;
    stimuli[5][20] = 0xB00B0012;
    stimuli[6][35] = 0xB01B0012;
    stimuli[10][50] = 0xB01B0112;
    stimuli[15][18] = 0xB10A0101;
    stimuli[19][33] = 0xA10A0101;
    stimuli[22][37] = 0xA10A1101;

    retval = dsched_add_entry(1, 28, &stimuli[1][28]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(5, 20, &stimuli[5][20]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(0, 0, &stimuli[0][0]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(19, 33, &stimuli[19][33]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(6, 35, &stimuli[6][35]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(10, 50, &stimuli[10][50]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(22, 37, &stimuli[22][37]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");
    retval = dsched_add_entry(15, 18, &stimuli[15][18]);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Add entry failed!");

    retval = dsched_get_schedule(&entries[0], &total_num);
    ck_assert_msg(retval == DSCHED_STATUS_SUCCESS, "Get schedule failed!");
    ck_assert_msg(total_num == 8, "Expected to get 8 entries!");

    ck_assert_msg((entries[0].hour * 60 + entries[0].minute) < (entries[1].hour * 60 + entries[1].minute), "entry[0] should be earlier than entry[1]");
    ck_assert_msg((entries[1].hour * 60 + entries[1].minute) < (entries[2].hour * 60 + entries[2].minute), "entry[1] should be earlier than entry[2]");
    ck_assert_msg((entries[2].hour * 60 + entries[2].minute) < (entries[3].hour * 60 + entries[3].minute), "entry[2] should be earlier than entry[3]");
    ck_assert_msg((entries[3].hour * 60 + entries[3].minute) < (entries[4].hour * 60 + entries[4].minute), "entry[3] should be earlier than entry[4]");
    ck_assert_msg((entries[4].hour * 60 + entries[4].minute) < (entries[5].hour * 60 + entries[5].minute), "entry[4] should be earlier than entry[5]");
    ck_assert_msg((entries[5].hour * 60 + entries[5].minute) < (entries[6].hour * 60 + entries[6].minute), "entry[5] should be earlier than entry[6]");
    ck_assert_msg((entries[6].hour * 60 + entries[6].minute) < (entries[7].hour * 60 + entries[7].minute), "entry[6] should be earlier than entry[7]");

    for (uint32_t i = 0; i < total_num; i++) {
        dsched_entry_data_t data = stimuli[entries[i].hour][entries[i].minute];
        ck_assert_msg(entries[i].data == data, "Got 0x%08x, expected 0x%08x at %02d:%02d", entries[i].data, data, entries[i].hour, entries[i].minute);
    }
}
END_TEST

START_TEST(test_get_schedule_empty)
{
    dsched_status_t retval;
    dsched_entry_t entries[DSCHED_ENTRIES_MAX];
    uint32_t total_num;

    dsched_init();

    retval = dsched_get_schedule(&entries[0], &total_num);
    ck_assert_msg(retval == DSCHED_STATUS_FAIL, "Expected fail but nothing happen!");
    ck_assert_msg(total_num == 0, "Schedule is not empty!");
}
END_TEST

Suite* module_suite(void)
{
    Suite* s;
    TCase* tc_add_entry;
    TCase* tc_remove_entry;
    TCase* tc_misc;

    s = suite_create("Module");

    // Core test case
    tc_add_entry = tcase_create("Add entry");
    tcase_add_test(tc_add_entry, test_add);
    tcase_add_test(tc_add_entry, test_add_once);
    tcase_add_test(tc_add_entry, test_add_one_before);
    tcase_add_test(tc_add_entry, test_add_one_after);
    tcase_add_test(tc_add_entry, test_add_overflow);
    tcase_add_test(tc_add_entry, test_add_collision);
    tcase_add_test(tc_add_entry, test_add_after_remove);
    suite_add_tcase(s, tc_add_entry);

    tc_remove_entry = tcase_create("Remove entry");
    tcase_add_test(tc_remove_entry, test_remove);
    tcase_add_test(tc_remove_entry, test_remove_underflow);
    tcase_add_test(tc_remove_entry, test_remove_nonexistent);
    suite_add_tcase(s, tc_remove_entry);

    tc_misc = tcase_create("Misc");
    tcase_add_test(tc_misc, test_get_schedule);
    tcase_add_test(tc_misc, test_get_schedule_empty);
    suite_add_tcase(s, tc_misc);

    return s;
}

int main(void)
{
    int number_failed;
    Suite* s;
    SRunner* sr;

    s = module_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
