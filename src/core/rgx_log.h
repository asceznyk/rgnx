#ifndef RGX_LOG_H
#define RGX_LOG_H

#include <rgx_core.h>

typedef enum {
  RGX_LOG_DEBUG,
  RGX_LOG_INFO,
  RGX_LOG_WARN,
  RGX_LOG_ERROR
} rgx_log_level_t;

extern rgx_log_level_t rgx_log_level;

void rgx_log_write(
  rgx_log_level_t level, const char *file, int line, const char *fmt, ...
);
rgx_int_t rgx_lvl_int(u_char *level);

void rgx_write_stderr(char *text);

#define rgx_log_put(level, ...) \
  rgx_log_write(level, __FILE__, __LINE__, __VA_ARGS__)

#define rgx_log_debug(...) rgx_log_put(RGX_LOG_DEBUG, __VA_ARGS__)
#define rgx_log_info(...)  rgx_log_put(RGX_LOG_INFO, __VA_ARGS__)
#define rgx_log_warn(...)  rgx_log_put(RGX_LOG_WARN, __VA_ARGS__)
#define rgx_log_error(...) rgx_log_put(RGX_LOG_ERROR, __VA_ARGS__)

#endif

