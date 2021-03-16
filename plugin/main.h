#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef MAIN
#define MAIN
#endif

#ifndef MAIN_API
#define MAIN_API
#endif

#ifndef MAIN_IMPL_API
#define MAIN_IMPL_API
#endif

#if defined(__cplusplus)
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#endif

#if defined(__cplusplus)
namespace fs = std::experimental::filesystem;
	#if defined(UNICODE)
		#define _tstring	std::wstring
	#else
		#define _tstring	std::string
	#endif
#endif

#include <windows.h>

#include <tchar.h>
#include <stdio.h>