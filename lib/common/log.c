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
#include <mqueue.h>
#include <fcntl.h>
#include <common.h>
#include <logd.h>
#include <errno.h>

static char g_override_tag[MAX_TAG_LEN] = {0};

static const char *get_process_name(void) {
#if defined(__GLIBC__) || defined(_GNU_SOURCE)
    if (program_invocation_short_name && *program_invocation_short_name) {
        return program_invocation_short_name;
    }
#endif
    return "waxl";
}

void log_set_tag(const char *tag) {
    if (tag && *tag) {
        strncpy(g_override_tag, tag, MAX_TAG_LEN - 1);
        g_override_tag[MAX_TAG_LEN - 1] = '\0';
    }
}

static const char *resolve_tag(const char *callsite_tag) {
    if (callsite_tag && *callsite_tag) {
        return callsite_tag;
    }
    if (g_override_tag[0] != '\0') {
        return g_override_tag;
    }
    const char *prog = get_process_name();
    if (*prog) {
        return prog;
    }
    return "waxl";
}

static const char* strip_path(const char* p) {
    const char* s = p;
    if (!s) return "";
    const char* last_slash = NULL;

    for (const char* it = s; *it; it++) {
        if (*it == '/') last_slash = it;
    }
    return last_slash ? last_slash + 1 : s;
}

void c_log_output_tagged(const char *tag, log_level_t level, const char *file, int line, const char *fmt, ...) {
    log_entry_t entry;
    time_t raw;
    time(&raw);
    struct tm *timeinfo = localtime(&raw);

    char t_str[9];
    strftime(t_str, sizeof(t_str), "%H:%M:%S", timeinfo);

    const char *lbl;
    const char *col;
    int sys_prio = LOGD_INFO;

    switch(level) {
        case LOG_DEBUG: lbl = "DBG"; col = "\033[36m"; sys_prio = LOGD_DEBUG; break;
        case LOG_INFO:  lbl = "INF"; col = "\033[32m"; sys_prio = LOGD_INFO;  break;
        case LOG_WARN:  lbl = "WRN"; col = "\033[33m"; sys_prio = LOGD_WARNING; break;
        case LOG_ERROR: lbl = "ERR"; col = "\033[31m"; sys_prio = LOGD_ERR;   break;
        default: return;
    }

    char screen_buf[MAX_MSG_LEN];
    const char *active_tag = resolve_tag(tag);
    int offset = 0;

    if (file != NULL) {
       const char* f2 = strip_path(file);
       offset = snprintf(screen_buf, sizeof(screen_buf),
                      "%s %s[%s]\033[0m %s: (%s:%d) ",
                      t_str, col, lbl, active_tag, f2, line);
    } else {
       offset = snprintf(screen_buf, sizeof(screen_buf),
                      "%s %s[%s]\033[0m %s: ",
                      t_str, col, lbl, active_tag);
    }

    va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);

    vsnprintf(screen_buf + offset, sizeof(screen_buf) - offset, fmt, args1);
    va_end(args1);

    size_t len = strlen(screen_buf);
    if (len > 0 && screen_buf[len - 1] != '\n') {
        if (len < sizeof(screen_buf) - 1) {
            screen_buf[len] = '\n';
            screen_buf[len + 1] = '\0';
        }
    }

    memset(&entry, 0, sizeof(entry));
    entry.priority = sys_prio;
    strncpy(entry.tag, active_tag, MAX_TAG_LEN - 1);

    int msg_offset = 0;
    if (file != NULL) {
        const char* f2 = strip_path(file);
        msg_offset = snprintf(entry.message, MAX_MSG_LEN, "(%s:%d) ", f2, line);
    }

    if (msg_offset < MAX_MSG_LEN) {
        vsnprintf(entry.message + msg_offset, MAX_MSG_LEN - msg_offset, fmt, args2);
    }
    va_end(args2);

    mqd_t mq = mq_open(MQ_NAME, O_WRONLY | O_NONBLOCK);
    bool sent_to_mq = false;

    if (mq != (mqd_t)-1) {
        if (mq_send(mq, (const char *)&entry, sizeof(entry), 0) == 0) {
            sent_to_mq = true;
        }
        mq_close(mq);
    }

    if (!sent_to_mq) {
        fputs(screen_buf, stderr);
        fflush(stderr);
    }
}
