/*
 * Copyright (C) 2009 The Android Open Source Project
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

#pragma once

/**
 * @addtogroup Logging
 * @{
 */

/**
 * \file
 *
 * Support routines to send messages to the Android log buffer,
 * which can later be accessed through the `logcat` utility.
 *
 * Each log message must have
 *   - a priority
 *   - a log tag
 *   - some text
 *
 * The tag normally corresponds to the component that emits the log message,
 * and should be reasonably small.
 *
 * Log message text may be truncated to less than an implementation-specific
 * limit (1023 bytes).
 *
 * Note that a newline character ("\n") will be appended automatically to your
 * log message, if not already there. It is not possible to send several
 * messages and have them appear on a single line in logcat.
 *
 * Please use logging in moderation:
 *
 *  - Sending log messages eats CPU and slow down your application and the
 *    system.
 *
 *  - The circular log buffer is pretty small, so sending many messages
 *    will hide other important log messages.
 *
 *  - In release builds, only send log messages to account for exceptional
 *    conditions.
 */

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Android log priority values, in increasing order of priority.
 */
typedef enum android_LogPriority {
  /** For internal use only.  */
  ANDROID_LOG_UNKNOWN = 0,
  /** The default priority, for internal use only.  */
  ANDROID_LOG_DEFAULT, /* only for SetMinPriority() */
  /** Verbose logging. Should typically be disabled for a release apk. */
  ANDROID_LOG_VERBOSE,
  /** Debug logging. Should typically be disabled for a release apk. */
  ANDROID_LOG_DEBUG,
  /** Informational logging. Should typically be disabled for a release apk. */
  ANDROID_LOG_INFO,
  /** Warning logging. For use with recoverable failures. */
  ANDROID_LOG_WARN,
  /** Error logging. For use with unrecoverable failures. */
  ANDROID_LOG_ERROR,
  /** Fatal logging. For use when aborting. */
  ANDROID_LOG_FATAL,
  /** For internal use only.  */
  ANDROID_LOG_SILENT, /* only for SetMinPriority(); must be last */
} android_LogPriority;

/**
 * Writes the constant string `text` to the log, with priority `prio` and tag
 * `tag`.
 */
int __android_log_write(int prio, const char* tag, const char* text);

/**
 * Writes a formatted string to the log, with priority `prio` and tag `tag`.
 * The details of formatting are the same as for
 * [printf(3)](http://man7.org/linux/man-pages/man3/printf.3.html).
 */
int __android_log_print(int prio, const char* tag, const char* fmt, ...)
#if defined(__GNUC__)
    __attribute__((__format__(printf, 3, 4)))
#endif
    ;

/**
 * Equivalent to `__android_log_print`, but taking a `va_list`.
 * (If `__android_log_print` is like `printf`, this is like `vprintf`.)
 */
int __android_log_vprint(int prio, const char* tag, const char* fmt, va_list ap)
#if defined(__GNUC__)
    __attribute__((__format__(printf, 3, 0)))
#endif
    ;

/**
 * Writes an assertion failure to the log (as `ANDROID_LOG_FATAL`) and to
 * stderr, before calling
 * [abort(3)](http://man7.org/linux/man-pages/man3/abort.3.html).
 *
 * If `fmt` is non-null, `cond` is unused. If `fmt` is null, the string
 * `Assertion failed: %s` is used with `cond` as the string argument.
 * If both `fmt` and `cond` are null, a default string is provided.
 *
 * Most callers should use
 * [assert(3)](http://man7.org/linux/man-pages/man3/assert.3.html) from
 * `&lt;assert.h&gt;` instead, or the `__assert` and `__assert2` functions
 * provided by bionic if more control is needed. They support automatically
 * including the source filename and line number more conveniently than this
 * function.
 */
void __android_log_assert(const char* cond, const char* tag, const char* fmt,
                          ...)
#if defined(__GNUC__)
    __attribute__((__noreturn__))
    __attribute__((__format__(printf, 3, 4)))
#endif
    ;

#ifndef log_id_t_defined
#define log_id_t_defined
/**
 * Identifies a specific log buffer for __android_log_buf_write()
 * and __android_log_buf_print().
 */
typedef enum log_id {
  LOG_ID_MIN = 0,

  /** The main log buffer. This is the only log buffer available to apps. */
  LOG_ID_MAIN = 0,
  /** The radio log buffer. */
  LOG_ID_RADIO = 1,
  /** The event log buffer. */
  LOG_ID_EVENTS = 2,
  /** The system log buffer. */
  LOG_ID_SYSTEM = 3,
  /** The crash log buffer. */
  LOG_ID_CRASH = 4,
  /** The statistics log buffer. */
  LOG_ID_STATS = 5,
  /** The security log buffer. */
  LOG_ID_SECURITY = 6,
  /** The kernel log buffer. */
  LOG_ID_KERNEL = 7,

  LOG_ID_MAX
} log_id_t;
#endif

/**
 * Writes the constant string `text` to the log buffer `id`,
 * with priority `prio` and tag `tag`.
 *
 * Apps should use __android_log_write() instead.
 */
int __android_log_buf_write(int bufID, int prio, const char* tag,
                            const char* text);

/**
 * Writes a formatted string to log buffer `id`,
 * with priority `prio` and tag `tag`.
 * The details of formatting are the same as for
 * [printf(3)](http://man7.org/linux/man-pages/man3/printf.3.html).
 *
 * Apps should use __android_log_print() instead.
 */
int __android_log_buf_print(int bufID, int prio, const char* tag,
                            const char* fmt, ...)
#if defined(__GNUC__)
    __attribute__((__format__(printf, 4, 5)))
#endif
    ;

#ifdef __cplusplus
}
#endif

/** @} */
