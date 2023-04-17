// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4607A810_33E2_483D_80D8_BE41F0D473D5__INCLUDED_)
#define AFX_STDAFX_H__4607A810_33E2_483D_80D8_BE41F0D473D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define DLLEXPORT __declspec(dllexport)
#define	DLLIMPORT __declspec(dllimport)

#include "stdio.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

// TODO: reference additional headers your program requires here
void Msg(char* szFormat, ...);
void dbMsg(char* szFormat, ...);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4607A810_33E2_483D_80D8_BE41F0D473D5__INCLUDED_)

