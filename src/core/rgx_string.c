#include <rgx_core.h>

rgx_str_t
rgx_str_dup(rgx_arena_t *arena, rgx_str_t src)
{
  rgx_str_t dst = {0};
  dst.data = rgx_arena_alloc(arena, src.len);
  if (dst.data == NULL) {
    return dst;
  }
  dst.len = src.len;
  rgx_memcpy(dst.data, src.data, src.len);
  return dst;
}

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

