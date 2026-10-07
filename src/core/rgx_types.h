#include <stdint.h>
#include <string.h>

typedef unsigned char u_char;

typedef intptr_t rgx_int_t;
typedef uintptr_t rgx_uint_t;

typedef struct {
  size_t len;
  u_char *data;
} rgx_str_t;


