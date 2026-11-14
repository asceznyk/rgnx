#include <rgx_core.h>
#include <rgnx.h>

static void rgx_show_version_info(void);
static rgx_int_t rgx_get_options(int argc, char *argv[]);

static rgx_uint_t rgx_show_help;
static rgx_uint_t rgx_show_version;
static u_char *rgx_conf_file;

static void
rgx_show_version_info(void) {
  rgx_write_stderr("rgnx version: " RGNX_VER_BUILD RGX_LINEFEED);
  if (rgx_show_help) {
    rgx_write_stderr(
      "Usage: rgnx [-?hvq] [-s signal]" RGX_LINEFEED
      "            [-c filename] [-e filename]" RGX_LINEFEED
      "Options:" RGX_LINEFEED
      "  -?,-h         : this help" RGX_LINEFEED
      "  -v            : show version and exit" RGX_LINEFEED
      "  -t            : test configuration and exit" RGX_LINEFEED
      "  -q            : suppress non-error messages "
                         "during configuration testing" RGX_LINEFEED
      "  -s signal     : send signal to a master process: "
                         "stop, quit, reopen, reload" RGX_LINEFEED
      "  -c filename   : set configuration file (default: NONE - will error out"
                         ")" RGX_LINEFEED
      "  -e filename   : set error log file (default: stderr)" RGX_LINEFEED
    );
  }
}

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
        case 'l':
          if (*p) {
            rgx_log_level = (rgx_log_level_t) rgx_lvl_int(p);
            goto next;
          }
          if (argv[++i]) {
            rgx_log_level = (rgx_log_level_t) rgx_lvl_int((u_char *)argv[i]);
            goto next;
          }
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
  if (rgx_show_version) {
    rgx_show_version_info();
    return 0;
  }
  return 0;
}


