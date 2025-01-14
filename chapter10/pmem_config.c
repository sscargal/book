/*
 * Copyright (c) 2019 Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY LOG OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* 
 * pmem_config.c - Demonstrates the use of several 
 *            configuration functions within 
 *            libmemkind.
 */ 

#include <memkind.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define PMEM_MAX_SIZE (1024 * 1024 * 32)

static char path[PATH_MAX] = "pmemfs//";

static void print_err_message(int err)
{
    char error_message[MEMKIND_ERROR_MESSAGE_SIZE];
    memkind_error_message(err, error_message, 
        MEMKIND_ERROR_MESSAGE_SIZE);
    fprintf(stderr, "%s\n", error_message);
}

int main(int argc, char *argv[])
{
    struct memkind *pmem_kind = NULL;
    int err = 0;

    if (argc > 2) {
        fprintf(stderr, 
            "Usage: %s [pmem_kind_dir_path]\n", 
            argv[0]);
        return 1;
    } else if (argc == 2 && 
        (realpath(argv[1], path) == NULL)) {
        fprintf(stderr, 
            "Incorrect pmem_kind_dir_path %s\n", 
            argv[1]);
        return 1;
    }

    fprintf(stdout,
            "This example shows how to use custom "
            "configuration to create pmem kind."
            "\nPMEM kind directory: %s\n", path);

    struct memkind_config *test_cfg =     
        memkind_config_new();
    if (!test_cfg) {
        fprintf(stderr, 
            "Unable to create memkind cfg.\n");
        return 1;
    }

    memkind_config_set_path(test_cfg, path);
    memkind_config_set_size(test_cfg, PMEM_MAX_SIZE);
    memkind_config_set_memory_usage_policy(test_cfg,
        MEMKIND_MEM_USAGE_POLICY_CONSERVATIVE);


    // Create PMEM partition with the configuration
    err =  memkind_create_pmem_with_config(test_cfg, 
        &pmem_kind);
    if (err) {
        print_err_message(err);
        return 1;
    }

    err = memkind_destroy_kind(pmem_kind);
    if (err) {
        print_err_message(err);
        return 1;
    }

    memkind_config_delete(test_cfg);

    fprintf(stdout,
        "PMEM kind and configuration was successfully"
        " created and destroyed.\n");

    return 0;
}
