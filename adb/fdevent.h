/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __FDEVENT_H
#define __FDEVENT_H

#include <stddef.h>
#include <stdint.h>  /* for int64_t */

#include <functional>
#include <variant>

#include "adb_unique_fd.h"

/* events that may be observed */
#define FDE_READ              0x0001
#define FDE_WRITE             0x0002
#define FDE_ERROR             0x0004

typedef void (*fd_func)(int fd, unsigned events, void *userdata);
typedef void (*fd_func2)(struct fdevent* fde, unsigned events, void* userdata);

struct fdevent {
    uint64_t id;

    unique_fd fd;
    int force_eof = 0;

    uint16_t state = 0;
    uint16_t events = 0;

    std::variant<fd_func, fd_func2> func;
    void* arg = nullptr;
};

// Allocate and initialize a new fdevent object
// TODO: Switch these to unique_fd.
fdevent *fdevent_create(int fd, fd_func func, void *arg);
fdevent* fdevent_create(int fd, fd_func2 func, void* arg);

// Deallocate an fdevent object that was created by fdevent_create.
void fdevent_destroy(fdevent *fde);

// fdevent_destroy, except releasing the file descriptor previously owned by the fdevent.
unique_fd fdevent_release(fdevent* fde);

// Change which events should cause notifications
void fdevent_set(fdevent *fde, unsigned events);
void fdevent_add(fdevent *fde, unsigned events);
void fdevent_del(fdevent *fde, unsigned events);

void fdevent_set_timeout(fdevent *fde, int64_t  timeout_ms);

// Loop forever, handling events.
void fdevent_loop();

void check_main_thread();

// Queue an operation to run on the main thread.
void fdevent_run_on_main_thread(std::function<void()> fn);

// The following functions are used only for tests.
void fdevent_terminate_loop();
size_t fdevent_installed_count();
void fdevent_reset();
void set_main_thread();

#endif
