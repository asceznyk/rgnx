#ifndef RGX_FILE_H
#define RGX_FILE_H

#include <rgx_core.h>

struct rgx_file_s {
  rgx_fd_t fd;
  rgx_str_t name;
  rgx_file_info_t info;
  off_t offset;
  off_t sys_offset;
  unsigned valid_info:1;
  unsigned directio:1;
};

#endif


