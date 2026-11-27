#include <rgx_core.h>

ssize_t
rgx_read_file(rgx_file_t *file, u_char *buf, size_t size, off_t offset)
{
  ssize_t n;
  n = pread(file->fd, buf, size, offset);
  if (n == -1) {
    rgx_log_error("rgx_read_file: error in `pread` syscall!");
    return RGX_ERROR;
  }
  file->offset += n;
  return n;
}

