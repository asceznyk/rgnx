#include <rgx_log.h>

rgx_log_level_t rgx_log_level = RGX_LOG_INFO;

static const char* rgx_lvl_str(rgx_log_level_t l);

static const char *
rgx_lvl_str(rgx_log_level_t l)
{
  switch (l) {
    case RGX_LOG_DEBUG: return "DEBUG";
    case RGX_LOG_INFO: return "INFO";
    case RGX_LOG_WARN: return "WARN";
    default: return "ERROR";
  }
}

rgx_int_t
rgx_lvl_int(u_char *level)
{
  if (strcmp((char *) level, "DEBUG") == 0)
    return RGX_LOG_DEBUG;
  if (strcmp((char *) level, "INFO") == 0)
    return RGX_LOG_INFO;
  if (strcmp((char *) level, "WARN") == 0)
    return RGX_LOG_WARN;
  if (strcmp((char *) level, "ERROR") == 0)
    return RGX_LOG_ERROR;
  return RGX_LOG_INFO;
}

void
rgx_write_stderr(char *text)
{
  write(rgx_stderr, text, rgx_strlen(text));
}

void
rgx_log_write(
  rgx_log_level_t level,
  const char *file,
  int line,
  const char *fmt,
  ...
)
{
  if (level < rgx_log_level) return;
  char msg[1024];
  time_t t = time(NULL);
  char timebuf[20];
  strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&t));
  int pos = snprintf(
    msg, sizeof(msg),
    "[%s] %s %s:%d: ", timebuf,
    rgx_lvl_str(level), file, line
  );
  va_list args;
  va_start(args, fmt);
  pos += vsnprintf(msg + pos, sizeof(msg) - pos, fmt, args);
  va_end(args);
  snprintf(msg + pos, sizeof(msg) - pos, "\n");
  fputs(msg, stderr);
}

