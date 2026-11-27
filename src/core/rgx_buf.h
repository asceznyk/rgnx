#ifndef RGX_BUF_H
#define RGX_BUF_H

#include <rgx_core.h>

typedef struct rgx_buf_s rgx_buf_t;

struct rgx_buf_s {
  u_char *pos;
  u_char *last;
  off_t file_pos;
  off_t file_last;
  u_char *start;
  u_char *end;
  rgx_file_t *file;
  unsigned temporary:1;
  unsigned memory:1;
  unsigned mmap:1;
  unsigned recycled:1;
  unsigned in_file:1;
  unsigned flush:1;
  unsigned sync:1;
  unsigned last_buf:1;
  unsigned last_in_chain:1;
  unsigned last_shadow:1;
  unsigned temp_file:1;
  int num;
};

#endif


