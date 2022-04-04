#ifndef SYSDEFINE_H
#define SYSDEFINE_H

#if(CIPAS_WIN32)
typedef char                CHAR, *P_CHAR;
typedef signed char         INT8, *P_INT8;
typedef unsigned char       UCHAR, *P_UCHAR;
typedef unsigned char       UINT8, *P_UINT8;
typedef unsigned char       BYTE, *P_BYTE;
typedef short               SHORT, *P_SHORT;
typedef signed short        INT16, *P_INT16;
typedef unsigned short      USHORT, *P_USHORT;
typedef unsigned short      UINT16, *P_UINT16;
typedef unsigned short      WORD, *P_WORD;
typedef int                 INT, *P_INT;
typedef signed int          INT32, *P_INT32;
typedef unsigned int        UINT, *P_UINT;
typedef unsigned int        UINT32, *P_UINT32;
typedef long                LONG, *P_LONG;
typedef unsigned long       ULONG, *P_ULONG;
typedef unsigned long       DWORD, *P_DWORD;
typedef __int64             LONGLONG, *P_LONGLONG;
typedef __int64             LONG64, *P_LONG64;
typedef signed __int64      INT64, *P_INT64;
typedef unsigned __int64    ULONGLONG, *P_ULONGLONG;
typedef unsigned __int64    DWORDLONG, *P_DWORDLONG;
typedef unsigned __int64    ULONG64, *P_ULONG64;
typedef unsigned __int64    DWORD64, *P_DWORD64;
typedef unsigned __int64    UINT64, *P_UINT64;
typedef double				DOUBLE, *P_DOUBLE;
typedef float				FLOAT, *P_FLOAT;
#else if(CIPAS_LINUX)
typedef char                CHAR, *P_CHAR;
typedef signed char         INT8, *P_INT8;
typedef unsigned char       UCHAR, *P_UCHAR;
typedef unsigned char       UINT8, *P_UINT8;
typedef unsigned char       BYTE, *P_BYTE;
typedef short               SHORT, *P_SHORT;
typedef signed short        INT16, *P_INT16;
typedef unsigned short      USHORT, *P_USHORT;
typedef unsigned short      UINT16, *P_UINT16;
typedef unsigned short      WORD, *P_WORD;
typedef int                 INT, *P_INT;
typedef signed int          INT32, *P_INT32;
typedef unsigned int        UINT, *P_UINT;
typedef unsigned int        UINT32, *P_UINT32;
typedef long                LONG, *P_LONG;
typedef unsigned long       ULONG, *P_ULONG;
typedef unsigned long       DWORD, *P_DWORD;
typedef __int64             LONGLONG, *P_LONGLONG;
typedef __int64             LONG64, *P_LONG64;
typedef signed __int64      INT64, *P_INT64;
typedef unsigned __int64    ULONGLONG, *P_ULONGLONG;
typedef unsigned __int64    DWORDLONG, *P_DWORDLONG;
typedef unsigned __int64    ULONG64, *P_ULONG64;
typedef unsigned __int64    DWORD64, *P_DWORD64;
typedef unsigned __int64    UINT64, *P_UINT64;
typedef double				DOUBLE, *P_DOUBLE;
typedef float				FLOAT, *P_FLOAT;


#endif

#endif