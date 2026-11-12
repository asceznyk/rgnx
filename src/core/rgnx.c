#include <rgx_core.h>
#include <rgnx.h>

static rgx_int_t rgx_get_options(int argc, char *argv[]);

static rgx_uint_t rgx_show_help;
static rgx_uint_t rgx_show_version;
static u_char *rgx_conf_file;

static rgx_int_t
rgx_get_options(int argc, char *argv[])
{
  rgx_uint_t i;
  u_char *p;
  for (i = 1; i < argc; i++) {
    p = (u_char *)argv[i];
    if (*p++ != '-') {
      rgx_log_error("rgx_get_options: invalid option \"%s\"", argv[i]);
      return RGX_ERROR;
    }
    while (*p) {
      switch(*p++) {
        case '?':
        case 'h':
          rgx_show_version = 1;
          rgx_show_help = 1;
          break;
        case 'c':
          if (*p) {
            rgx_conf_file = p;
            goto next;
          }
          if (argv[++i]) {
            rgx_conf_file = (u_char *)argv[i];
            goto next;
          }
          rgx_log_error("rgx_get_options: option \"-c\" requires file name");
          return RGX_ERROR;
        case 'v':
          rgx_show_version = 1;
          break;
        default:
          rgx_log_error("rgx_get_options: invalid option \"%c\"", *(p-1));
          return RGX_ERROR;
      }
    }
    next:
      continue;
  }
  return RGX_OK;
}

int
main(int argc, char *argv[])
{
  if (rgx_get_options(argc, argv) != RGX_OK)
    return 1;
  return 0;
}


