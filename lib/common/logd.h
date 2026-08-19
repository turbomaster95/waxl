#ifndef LOG_COMMON_H
#define LOG_COMMON_H

#define MQ_NAME "/waxl_log"
#define MAX_TAG_LEN 32
#define MAX_MSG_LEN 256

#define LOGD_EMERG   0   /* System is unusable */
#define LOGD_ALERT   1   /* Action must be taken immediately */
#define LOGD_CRIT    2   /* Critical conditions */
#define LOGD_ERR     3   /* Error conditions */
#define LOGD_WARNING 4   /* Warning conditions */
#define LOGD_NOTICE  5   /* Normal but significant condition */
#define LOGD_INFO    6   /* Informational */
#define LOGD_DEBUG   7   /* Debug-level messages */

typedef struct {
    int priority;
    char tag[MAX_TAG_LEN];
    char message[MAX_MSG_LEN];
} log_entry_t;

#endif
