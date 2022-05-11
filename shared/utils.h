#ifndef UTILS_H
#define UTILS_H

#if defined(_WIN32) || defined(__CYGWIN__)
#include "shlobj_core.h "
    #define HasAdminRights() IsUserAnAdmin()
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#include <sys/types.h>
    #define HasAdminRights() (geteuid() == 0)
#endif

#endif
