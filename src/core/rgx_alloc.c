#include <rgx_core.h>

void *
rgx_malloc(size_t size)
{
  rgx_log_info("rgx_malloc: size = %zu", size);
  void *p = malloc(size);
  if (p == NULL)
    rgx_log_error("rgx_malloc: FAILED malloc(%zu)", size);
  return p;
}

void *
rgx_calloc(size_t size)
{
  rgx_log_info("rgx_calloc: size = %zu", size);
  void *p  = rgx_malloc(size);
  if (p == NULL) {
    rgx_log_error("rgx_calloc: FAILED calloc(%zu)", size);
  }
  if (p)
    rgx_memzero(p, size);
  return p;
}

void *
rgx_memalign(size_t align, size_t size)
{
  void *p;
  int err = posix_memalign(&p, align, size);
  if (err) {
    rgx_log_error("rgx_memalign: posix_memalign(%zu, %zu) FAILED!",
                  align, size);
    p = NULL;
  }
  rgx_log_info("rgx_memalign: assigned p = %p", p);
  return p;
}

void
rgx_free(void *p)
{
  if (p == NULL) {
    rgx_log_error("rgx_free: p is already NULL");
    return;
  }
  free(p);
}


