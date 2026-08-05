#ifndef COMMON_H
#define COMMON_H

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} log_level_t;

void c_log_output(log_level_t level, const char *file, int line, const char *fmt, ...);

#define L_DEBUG(fmt, ...) c_log_output(LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define L_INFO(fmt, ...)  c_log_output(LOG_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define L_WARN(fmt, ...)  c_log_output(LOG_WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define L_ERROR(fmt, ...) c_log_output(LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif

