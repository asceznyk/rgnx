#include <rgx_core.h>

static rgx_int_t rgx_conf_read_token(rgx_conf_t *conf);

static rgx_int_t
rgx_conf_read_token(rgx_conf_t *conf)
{
  u_char *start, ch;
  off_t file_size;
  rgx_uint_t found, need_space, last_space, sharp_comment, variable;
  rgx_uint_t quoted, s_quoted, d_quoted, start_line;
  rgx_buf_t *buf, *dump;
  rgx_str_t *word;
  found = 0;
  need_space = 0;
  last_space = 1;
  sharp_comment = 0;
  variable = 0;
  quoted = 0;
  s_quoted = 0;
  d_quoted = 0;
  conf->args->nelts = 0;
  buf = conf->conf_file->buffer;
  dump = conf->conf_file->dump;
  start = buf->pos;
  start_line = conf->conf_file->line;
  file_size = rgx_file_size(&conf->conf_file->file.info);
  for (;;) {
    if (buf->pos >= buf->last) {
      if (conf->conf_file->file.offset >= file_size) {
        if (conf->args->nelts > 0 || !last_space) {
          if (conf->conf_file->file.fd == RGX_INVALID_FILE) {
            rgx_log_error(
              "rgx_conf_read_token: unexpected end of argument! missing \";\""
            );
            return RGX_ERROR;
          }
          rgx_log_error(
            "rgx_conf_read_token: unexpected EOF! \";\" or \"}\" is missing"
          );
          return RGX_ERROR;
        }
        return RGX_CONF_FILE_DONE;
      }
    }
    ch = *buf->pos++;
    if (ch == LF) {
      conf->conf_file->line++;
      if (sharp_comment)
        sharp_comment = 0;
    }
    if (sharp_comment)
      continue;
    if (quoted) {
      quoted = 0;
      continue;
    } //TODO: rest of the lexing logic
  }
  return RGX_ERROR;
}

char *
rgx_conf_parse(rgx_conf_t *conf, rgx_str_t *filename)
{
  rgx_fd_t fd;
  rgx_int_t rc;
  rgx_buf_t buf;
  rgx_conf_file_t *prev, conf_file;
  enum {
    parse_file = 0,
    parse_block,
    parse_param
  } type;
  if (filename) {
    fd = rgx_open_file(filename->data, RGX_FILE_RDONLY, RGX_FILE_OPEN, 0);
    if (fd == RGX_INVALID_FILE) {
      rgx_log_error("rgx_conf_parse: INVALID file");
      return RGX_CONF_ERROR;
    }
    prev = conf->conf_file;
    conf->conf_file = &conf_file;
    if (rgx_fd_info(fd, &conf->conf_file->file.info) == RGX_FILE_ERROR) {
      rgx_log_error(
        "rgx_conf_parse: rgx_fd_info on \"%s\" failed",
        filename->data
      );
    }
    conf->conf_file->buffer = &buf;
    buf.start = rgx_malloc(RGX_CONF_BUFFER);
    buf.pos = buf.start;
    buf.last = buf.start;
    buf.end = buf.start + RGX_CONF_BUFFER;
    buf.temporary = 1;
    conf->conf_file->line = 1;
    conf->conf_file->file.fd = fd;
    conf->conf_file->file.name.len = filename->len;
    conf->conf_file->file.name.data = filename->data;
    conf->conf_file->file.offset = 0;
    type = parse_file;
  } else if (conf->conf_file->file.fd != RGX_INVALID_FILE) {
    type = parse_block;
  } else {
    type = parse_param;
  } //TODO: loop for dispatch
failed:
  rc = RGX_ERROR;
done:
  if (filename) {
    if (conf->conf_file->buffer->start)
      rgx_free(conf->conf_file->buffer->start);
    if (rgx_close_file(fd) == RGX_FILE_ERROR) {
      rgx_log_error("rgx_conf_parse: close file failed!");
      rc = RGX_ERROR;
    }
  }
  if (rc == RGX_ERROR)
    return RGX_CONF_ERROR;
  return RGX_CONF_OK;
}


