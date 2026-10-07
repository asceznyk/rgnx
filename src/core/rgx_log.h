#ifndef RGX_LOG_H
#define RGX_LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

typedef enum { RGX_LOG_DEBUG, RGX_LOG_INFO, RGX_LOG_WARN, RGX_LOG_ERROR } rgx_log_level_t;

extern rgx_log_level_t g_log_level;

void rgx_log_write(rgx_log_level_t level, const char *file, int line, const char *fmt, ...);

#define rgx_log_debug(...) rgx_log_write(RGX_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define rgx_log_info(...)  rgx_log_write(RGX_LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define rgx_log_warn(...)  rgx_log_write(RGX_LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define rgx_log_error(...) rgx_log_write(RGX_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif

