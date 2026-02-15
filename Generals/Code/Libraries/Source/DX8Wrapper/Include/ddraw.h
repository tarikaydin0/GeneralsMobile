#ifndef _DDRAW_H_
#define _DDRAW_H_

#include "windows.h"

// Stub for ddraw.h to satisfy includes in ddsfile.cpp
// We don't actually need DirectDraw functionality for the Android port if it's just for DDS structure definitions

#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) |       \
                ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24 ))
#endif

#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))

#define DDSCAPS2_CUBEMAP 0x00000200L
#define DDSCAPS2_VOLUME  0x00200000L

#endif // _DDRAW_H_
