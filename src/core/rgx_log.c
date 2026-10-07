#include <rgx_log.h>

rgx_log_level_t g_log_level = RGX_LOG_INFO;

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

void
rgx_log_write(
  rgx_log_level_t level,
  const char *file,
  int line,
  const char *fmt,
  ...
)
{
  if (level < g_log_level) return;
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

