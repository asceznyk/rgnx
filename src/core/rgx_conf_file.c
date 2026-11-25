#include <rgx_core.h>

static rgx_int_t rgx_conf_read_token(rgx_conf_t *conf);

static rgx_int_t
rgx_conf_read_token(rgx_conf_t *conf)
{
  //rgx_buf_t *buf;
  return RGX_ERROR;
}

char *
rgx_conf_parse(rgx_conf_t *conf, rgx_str_t *filename)
{
  rgx_fd_t fd;
  enum {
    parse_file = 0,
    parse_block,
    parse_param
  } type;
  if (filename) {
    fd = rgx_open_file(filename->data, RGX_FILE_RDONLY, RGX_FILE_OPEN, 0);
    rgx_log_info("rgx_conf_parse: fd = %d", fd);
    if (fd == RGX_INVALID_FILE) {
      rgx_log_error("rgx_conf_parse: INVALID file");
      return RGX_CONF_ERROR;
    }
  } //TODO: more stuff!
  return RGX_CONF_OK;
}


