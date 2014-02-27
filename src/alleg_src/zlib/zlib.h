#ifndef _zlib_H_
#define _zlib_H_

//////////////////////////////////////////////////////////////////////////////
//
// zlib.H
//
//////////////////////////////////////////////////////////////////////////////

// warning C4355: 'this' : used in base member initializer list
#pragma warning(disable:4355)

#if !defined (_WIN64) 
	#define FLOATASM //Imago guarded from x64 compilers on 6/20/09
	#define USEASM
#endif

// VS.Net 2003 port: typename is required for template
#if _MSC_VER >= 1310
	#define VSNET_TNFIX typename
#else
	#define VSNET_TNFIX
#endif
//
// Required CRT headers
//

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "malloc.h"

//
// Required Win32 headers
//
#ifdef WIN
#include "windows.h"
#include "wininet.h"
#endif
//
// DirectX Stuff
//

#ifdef WIN
// KGJV - Removed dependancy from DX
#include "OldDXDefns.h"	// Older DX definitions included to get the project to build. At some point,
						// they all become redundant.
#endif
//
// Library Headers
//

#include "zlib/zassert.h"
#include "zlib/alloc.h"
#include "zlib/mask.h"
#include "zlib/tref.h"
#include "zlib/cast.h"
#include "zlib/zmath.h"
#include "zlib/zadapt.h"

#include "zlib/tcompare.h"
#include "zlib/tarray.h"
#include "zlib/tvector.h"
#include "zlib/tstack.h"
#include "zlib/tlist.h"
#include "zlib/tmap.h"

#include "zlib/comobj.h"

//
// Types
//

#include "zlib/ztime.h"
#include "zlib/zstring.h"
#include "zlib/point.h"
#include "zlib/rect.h"
#include "zlib/vector.h"
#include "zlib/vertex.h"
#include "zlib/orientation.h"
#include "zlib/matrix.h"
#include "zlib/quaternion.h"
#include "zlib/color.h"


//
// Interfaces
//

#include "zlib/event.h"
#include "zlib/input.h"
#include "zlib/genericlist.h"
#include "zlib/FTPSession.h"

//
// Win32 Wrapper Classes
//

#include "zlib/base.h"
#ifdef WIN
#include "zlib/window.h"
#include "zlib/Win32app.h"
#include "zlib/VersionInfo.h"
#endif

//
// Parser
//

#include "zlib/token.h"

#endif
