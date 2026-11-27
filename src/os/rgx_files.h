#ifndef RGX_FILES_H
#define RGX_FILES_H

#include <rgx_core.h>

typedef int rgx_fd_t;
typedef struct stat rgx_file_info_t;

#define RGX_INVALID_FILE -1
#define RGX_FILE_ERROR -1

#define RGX_FILE_OPEN 0
#define RGX_FILE_RDONLY O_RDONLY
#define RGX_FILE_WRONLY O_WRONLY
#define RGX_FILE_RDWR O_RDWR
#define RGX_FILE_CREATE_OR_OPEN O_CREAT

#define rgx_open_file(name, mode, create, access) \
  open((const char *)name, mode|create, access)

#define rgx_close_file close

#define rgx_file_size(sb) (sb)->st_size

#endif
