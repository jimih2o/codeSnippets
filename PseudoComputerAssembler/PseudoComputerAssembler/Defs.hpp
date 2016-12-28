#pragma once // is effectively std

typedef unsigned char Byte ;
typedef Byte BYTE ;
typedef BYTE Uint8 ;
typedef signed char SByte ;
typedef SByte Int8 ;
typedef unsigned Uint32 ;
typedef int Int32 ;

#ifdef __cplusplus
typedef bool Boolean ;
#else
typedef BYTE Boolean ;
typedef Boolean bool ;
#endif

typedef Boolean Bool ;

// Debugging utilities

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define NOMINMAX
#include <Windows.h> // I'm terribly sorry.
#undef near
#undef far

#define ASSERT(x, msg) do { \
	if (x) { \
	} else { \
	MessageBoxA(0, msg, msg, MB_OK) ; \
	__debugbreak() ; \
	} \
} while (0) 

#define ERROR_IF(x, msg) do { \
	if (x) { \
	MessageBoxA(0, msg, msg, MB_OK) ; \
	__debugbreak() ; \
	} \
} while (0) 

#else
#define ASSERT(x)   (x)
#define ERROR_IF(x) (x)
#endif