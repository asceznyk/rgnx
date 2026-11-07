#include <rgx_core.h>

rgx_int_t
rgx_atoi(u_char *line, size_t n)
{
  if (n == 0) {
    rgx_log_error("rgx_atoi: n == 0?!");
    return RGX_ERROR;
  }
  rgx_int_t i = 0;
  rgx_int_t v;
  for (v = 0; i < n; i++) {
    if (line[i] < '0' || line[i] > '9') {
      rgx_log_error("rgx_atoi: `%c` is not a number!", line[i]);
      return RGX_ERROR;
    }
    v = v * 10 + (line[i] - '0');
  }
  return v;
}

