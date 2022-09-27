#pragma once
#ifndef _YSFIO_UTIL_
#define _YSFIO_UTIL_
#include <stdio.h>
#include <cstring>

#define GET_REF2DATA(type, ref, data) type* pref = dynamic_cast<type*>(&data);if (nullptr == pref) {return nullptr;} type& ref = dynamic_cast<type&>(data);

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
