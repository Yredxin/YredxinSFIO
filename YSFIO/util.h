#pragma once
#ifndef _YSFIO_UTIL_
#define _YSFIO_UTIL_
#include <stdio.h>
#include <cstring>
#define DEBUG
#ifdef DEBUG
#define LOG(info) do { fprintf(stderr, "%s:%s:%d ==> %s %s", \
					__FILE__, __FUNCTION__, __LINE__, info, strerror(errno)); \
					perror(""); \
				  }while (0)
#else
#define LOG(info)
#endif // DEBUG

#endif // !_YSFIO_UTIL_
