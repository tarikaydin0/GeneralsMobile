#ifndef _OBJBASE_H_
#define _OBJBASE_H_

#include "windows.h"

// HRESULT values are in windows.h

#define DECLARE_INTERFACE(iface)    interface iface
#define DECLARE_INTERFACE_(iface, baseiface)    interface iface : public baseiface

#ifndef interface
#define interface struct
#endif

#ifndef REFGUID
typedef const GUID& REFGUID;
#endif

#ifndef REFIID
typedef const GUID& REFIID;
#endif

interface IUnknown {
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)() PURE;
    STDMETHOD_(ULONG, Release)() PURE;
};

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#endif // _OBJBASE_H_
