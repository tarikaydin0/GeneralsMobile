#ifndef _WINDOWS_H_
#define _WINDOWS_H_

#include <stdint.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Basic types
typedef void* HWND;
typedef void* HINSTANCE;
typedef void* HDC;
typedef void* HGLRC;
typedef void* HMENU;
typedef void* HICON;
typedef void* HBRUSH;
typedef void* HBITMAP;
typedef void* HANDLE;

typedef int INT;
typedef uint32_t DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned char* LPBYTE;
typedef BYTE* PBYTE;
typedef void* LPGLYPHMETRICSFLOAT;
typedef unsigned short WORD;
typedef float FLOAT;

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

#ifndef LONG
typedef int32_t LONG;
#endif
typedef uint32_t UINT;
typedef uintptr_t UINT_PTR;
typedef char CHAR;
typedef short SHORT;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef uint32_t ULONG;
typedef int64_t LONGLONG;
typedef uint64_t ULONGLONG;
typedef int32_t HRESULT;
typedef void* LPVOID;
typedef void* LPVoid;
typedef void VOID;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef const void* LPCVOID;
typedef void* HMONITOR;
typedef void* HMODULE;
typedef void* HFONT;
typedef DWORD* LPDWORD;
typedef void* PVOID;

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG  HighPart;
    };
    struct {
        DWORD LowPart;
        LONG  HighPart;
    } u;
    long long QuadPart;
} LARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;

typedef wchar_t WCHAR;
typedef const WCHAR* LPCWSTR;
typedef WCHAR* LPWSTR;

struct IStream;

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifdef UNICODE
typedef WCHAR TCHAR;
typedef LPCWSTR LPCTSTR;
typedef LPWSTR LPTSTR;
#else
typedef char TCHAR;
typedef LPCSTR LPCTSTR;
typedef LPSTR LPTSTR;
#endif

// CONSTants
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// HRESULT values
#define S_OK ((HRESULT)0L)
#define S_FALSE ((HRESULT)1L)
#define E_FAIL ((HRESULT)0x80004005L)
#define E_INVALIDARG ((HRESULT)0x80070057L)
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#define E_NOTIMPL ((HRESULT)0x80004001L)
#define E_NOINTERFACE ((HRESULT)0x80004002L)
#define E_POINTER ((HRESULT)0x80004003L)
#define E_HANDLE ((HRESULT)0x80070006L)
#define E_ABORT ((HRESULT)0x80004004L)
#define E_ACCESSDENIED ((HRESULT)0x80070005L)
#define E_PENDING ((HRESULT)0x8000000AL)
#define E_UNEXPECTED ((HRESULT)0x8000FFFFL)

#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#define MAKE_HRESULT(sev,fac,code) \
    ((HRESULT) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )

// Macros
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))
#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xFFFF))
#define HIBYTE(w)           ((BYTE)((((WORD)(w)) >> 8) & 0xFF))
#define LOBYTE(w)           ((BYTE)(((WORD)(w)) & 0xFF))

#define WINAPI
#define CALLBACK
#ifndef _REENTRANT
#define _REENTRANT
#endif
#define APIENTRY
#define STDMETHODCALLTYPE
#define STDMETHOD(method)       virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method
#define PURE                    = 0
#define THIS_
#define THIS                    void
#define DECLSPEC_UUID(x)
#define STDMETHODIMP            HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)     type STDMETHODCALLTYPE

#define STDAPI                  EXTERN_C HRESULT
#define STDAPI_(type)           EXTERN_C type
#define DECLARE_HANDLE(name) typedef void* name
#ifndef CONST
#define CONST const
#endif

// Structs
typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, *PRECT, *LPRECT;

typedef struct tagPOINT {
    LONG x;
    LONG y;
} POINT, *PPOINT;

typedef struct tagSIZE {
    LONG cx;
    LONG cy;
} SIZE, *PSIZE;

typedef struct tagPALETTEENTRY {
    BYTE peRed;
    BYTE peGreen;
    BYTE peBlue;
    BYTE peFlags;
} PALETTEENTRY, *PPALETTEENTRY, *LPPALETTEENTRY;

typedef struct _RGNDATAHEADER {
    DWORD dwSize;
    DWORD iType;
    DWORD nCount;
    DWORD nRgnSize;
    RECT  rcBound;
} RGNDATAHEADER, *PRGNDATAHEADER;

typedef struct _RGNDATA {
    RGNDATAHEADER rdh;
    char          Buffer[1];
} RGNDATA, *PRGNDATA, *LPRGNDATA;

typedef struct tagLOGFONTA {
    LONG lfHeight;
    LONG lfWidth;
    LONG lfEscapement;
    LONG lfOrientation;
    LONG lfWeight;
    BYTE lfItalic;
    BYTE lfUnderline;
    BYTE lfStrikeOut;
    BYTE lfCharSet;
    BYTE lfOutPrecision;
    BYTE lfClipPrecision;
    BYTE lfQuality;
    BYTE lfPitchAndFamily;
    CHAR lfFaceName[32];
} LOGFONTA, *PLOGFONTA, *LPLOGFONTA;

typedef struct tagLOGFONTW {
    LONG lfHeight;
    LONG lfWidth;
    LONG lfEscapement;
    LONG lfOrientation;
    LONG lfWeight;
    BYTE lfItalic;
    BYTE lfUnderline;
    BYTE lfStrikeOut;
    BYTE lfCharSet;
    BYTE lfOutPrecision;
    BYTE lfClipPrecision;
    BYTE lfQuality;
    BYTE lfPitchAndFamily;
    WCHAR lfFaceName[32];
} LOGFONTW, *PLOGFONTW, *LPLOGFONTW;

#ifdef UNICODE
typedef LOGFONTW LOGFONT;
#else
typedef LOGFONTA LOGFONT;
#endif

// GUID
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} GUID, *LPGUID;

#ifndef REFGUID
#ifdef __cplusplus
#define REFGUID const GUID &
#else
#define REFGUID const GUID * const
#endif
#endif

#ifndef LPCGUID
typedef const GUID* LPCGUID;
#endif
#endif

#include <string.h>

#ifdef _ANDROID
#define _strdup strdup
inline char* lstrcpyn(char* dest, const char* src, int n) {
    if (n > 0) {
        strncpy(dest, src, n - 1);
        dest[n - 1] = '\0';
    }
    return dest;
}
inline char* lstrcat(char* dest, const char* src) {
    return strcat(dest, src);
}
#endif

#include <android/log.h>

typedef void* HKEY;
typedef void* LPDISPATCH;

#ifndef GWL_STYLE
#define GWL_STYLE (-16)
#endif

#ifndef HWND_TOPMOST
#define HWND_TOPMOST ((HWND)-1)
#endif

#ifndef SWP_NOSIZE
#define SWP_NOSIZE 0x0001
#endif

#ifndef SWP_NOMOVE
#define SWP_NOMOVE 0x0002
#endif

#ifndef MONITOR_DEFAULTTOPRIMARY
#define MONITOR_DEFAULTTOPRIMARY 0x00000001
#endif

typedef struct tagMONITORINFO {
    DWORD cbSize;
    RECT  rcMonitor;
    RECT  rcWork;
    DWORD dwFlags;
} MONITORINFO, *LPMONITORINFO;

inline void ZeroMemory(void* p, size_t s) { memset(p, 0, s); }

inline LONG GetWindowLong(HWND hWnd, int nIndex) { return 0; }
inline BOOL GetClientRect(HWND hWnd, LPRECT lpRect) { if (lpRect) { lpRect->left = lpRect->top = 0; lpRect->right = 1920; lpRect->bottom = 1080; } return TRUE; }
inline BOOL AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu) { return TRUE; }
inline BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) { return TRUE; }
inline BOOL GetMonitorInfo(HMONITOR hMonitor, LPMONITORINFO lpmi) { return TRUE; }
inline HMONITOR MonitorFromWindow(HWND hwnd, DWORD dwFlags) { return (HMONITOR)0x1234; }

#endif // _WINDOWS_H_
