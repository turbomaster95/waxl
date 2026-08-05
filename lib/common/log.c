/*
 *            The Nicense Software License
 *                  Version 1.1
 *
 * Copyright (c) 2026 Deva Midhun. All rights reserved.
 *
 * The full LICENSE file is provided in the project root/licenses/NICENSE
 * This file was originally provided with libnu.
 */

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <common.h>

static const char* strip_path(const char* p) {
    const char* s = p;
    if (!s) return "";
    const char* last_slash = NULL;

    for (const char* it = s; *it; it++) {
        if (*it == '/') last_slash = it;
    }
    return last_slash ? last_slash + 1 : s;
}


void c_log_output(log_level_t level, const char *file, int line, const char *fmt, ...) {
    time_t raw;
    time(&raw);
    struct tm *timeinfo = localtime(&raw);

    char t_str[9];
    strftime(t_str, sizeof(t_str), "%H:%M:%S", timeinfo);

    const char *lbl;
    const char *col;
    switch(level) {
        case LOG_DEBUG: lbl = "DBG"; col = "\033[36m"; break; // Cyan
        case LOG_INFO:  lbl = "INF"; col = "\033[32m"; break; // Green
        case LOG_WARN:  lbl = "WRN"; col = "\033[33m"; break; // Yellow
        case LOG_ERROR: lbl = "ERR"; col = "\033[31m"; break; // Red
        default: return;
    }

    if (file != NULL) {
        const char* f2 = strip_path(file);
        fprintf(stderr, "%s %s[%s]\033[0m (%s:%d) ", t_str, col, lbl, f2, line);
    } else {
        fprintf(stderr, "%s %s[%s]\033[0m ", t_str, col, lbl);
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
}
