#ifndef _RGNX_H_INCLUDED_
#define _RGNX_H_INCLUDED_

#define rgnx_version      1000000
#define RGNX_VERSION      "1.0.0"
#define RGNX_VER          "rgnx/" RGNX_VERSION

#ifdef RGX_BUILD
#define RGNX_VER_BUILD    RGNX_VER " (" RGX_BUILD ")"
#else
#define RGNX_VER_BUILD    RGNX_VER
#endif

#define RGNX_VAR          "RGNX"
#define RGX_OLDPID_EXT     ".oldbin"

#endif /* _RGNX_H_INCLUDED_ */

