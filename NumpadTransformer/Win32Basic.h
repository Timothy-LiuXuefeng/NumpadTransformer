#pragma once

#ifndef WIN32_BASIC_H

#define WIN32_BASIC_H

#ifdef __cplusplus

#include <string>
#ifndef _tstring
#ifdef UNICODE
#define _tstring wstring
#define to_tstring to_wstring
#else	// UNICODE
#define _tstring string
#define to_tstring to_string
#endif	// UNICODE
#endif	// !_tstring

#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#define WIN32_NAMESPACE_BEGIN namespace Win32 {
#define WIN32_NAMESPACE_END }

#else	// __cplusplus
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#define WIN32_NAMESPACE_BEGIN
#define WIN32_NAMESPACE_END

#endif	// __cplusplus


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif	// #ifndef WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>

#endif	//#ifndef WIN32_BASIC_H
