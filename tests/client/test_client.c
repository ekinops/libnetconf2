/**
 * @file test_client.c
 * @author David Sedlák <xsedla1d@stud.fit.vutbr.cz>
 * @brief client test
 *
 * Copyright (c) 2018 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#define _GNU_SOURCE

#include <errno.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cmocka.h>
#include <libyang/libyang.h>
#include <log.h>
#include <session_client.h>
#include "tests/config.h"

static int
setup_f(void **state)
{
    (void)state;

    nc_verbosity(NC_VERB_VERBOSE);

    return 0;
}

static int
teardown_f(void **state)
{
    (void)state;

    return 0;
}

static void
test_nc_client_setting_schema_searchpath(void **state)
{
    (void)state;
    const char *path;
    int ret;

    /* initiate client */
    nc_client_init();

    path = nc_client_get_schema_searchpath();
    assert_null(path);

    ret = nc_client_set_schema_searchpath("path");
    assert_int_equal(ret, 0);
    path = nc_client_get_schema_searchpath();
    assert_string_equal(path, "path");

    ret = nc_client_set_schema_searchpath("path1");
    assert_int_equal(ret, 0);
    path = nc_client_get_schema_searchpath();
    assert_string_equal(path, "path1");
}

LY_ERR
test_clb(const char *mod_name, const char *mod_rev, const char *submod_name, const char *sub_rev, void *user_data,
        LYS_INFORMAT *format, const char **model_data, void (**free_module_data)(void *model_data, void *user_data))
{
    (void)mod_name;
    (void)mod_rev;
    (void)submod_name;
    (void)sub_rev;
    (void)user_data;
    (void)format;
    (void)model_data;
    (void)free_module_data;

    return LY_SUCCESS;
}

LY_ERR
test_clb1(const char *mod_name, const char *mod_rev, const char *submod_name, const char *sub_rev, void *user_data,
        LYS_INFORMAT *format, const char **model_data, void (**free_module_data)(void *model_data, void *user_data))
{
    (void)mod_name;
    (void)mod_rev;
    (void)submod_name;
    (void)sub_rev;
    (void)user_data;
    (void)format;
    (void)model_data;
    (void)free_module_data;

    return LY_SUCCESS;
}

static void
test_nc_client_setting_schema_callback(void **state)
{
    (void)state;
    ly_module_imp_clb ret_f;
    char *data_ret;
    int ret;

    ret_f = nc_client_get_schema_callback((void **)&data_ret);
    assert_null(ret_f);
    assert_null(data_ret);

    ret = nc_client_set_schema_callback(test_clb, "DATA");
    assert_int_equal(ret, 0);
    ret_f = nc_client_get_schema_callback((void **)&data_ret);
    assert_ptr_equal(test_clb, ret_f);
    assert_string_equal("DATA", data_ret);

    ret = nc_client_set_schema_callback(test_clb1, "DATA1");
    assert_int_equal(ret, 0);
    ret_f = nc_client_get_schema_callback((void **)&data_ret);
    assert_ptr_equal(test_clb1, ret_f);
    assert_string_equal("DATA1", data_ret);

    /* destroy client */
    nc_client_destroy();
}

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_nc_client_setting_schema_searchpath, setup_f, teardown_f),
        cmocka_unit_test_setup_teardown(test_nc_client_setting_schema_callback, setup_f, teardown_f),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
