/************************************************************************/
/*     ResLibrary.h --- Resource Library Definition for Resource ID		*/
/************************************************************************/
#pragma once

#ifdef RESLIBRARY_EXPORTS
#define RESLIBRARY_API __declspec(dllexport)
// Version
#define VERSION_NUMBER						2,1,0,11
#define VERSION_FILEVERSION					"2.1.0.11"
#define VERSION_BUILDTIME					"2014/2/18"
#ifdef _DEBUG
#define VERSION_PRODUCTVERSION 				"Ver.2.1 for Debug"
#define	VERSION_FILEDESCRIPTION				"Resource Debug Library"
#define	VERSION_PRODUCTNAME					"Standed Game Library for Debug"
#else
#define VERSION_PRODUCTVERSION 				"Ver.2.1"
#define	VERSION_FILEDESCRIPTION				"Resource Library"
#define	VERSION_PRODUCTNAME					"Standed Game Library"
#endif
#define VERSION_INTERNALNAME				"ResLibrary.dll"
#define VERSION_COMPANYNAME 				"SoaringLoong, Inc."
#define	VERSION_LEGALCOPYRIGHT				"Copyright (C) 2013 SoaringLoong, Inc."
#else
#define RESLIBRARY_API __declspec(dllimport)
#endif

class CLinkList;
extern RESLIBRARY_API CLinkList* g_pButtonResList;
extern RESLIBRARY_API CLinkList* g_pBackGroundResList;

extern LRESULT Initialize();
extern void Shutdown();

RESLIBRARY_API LRESULT GetButtonResourceID( LPCTSTR szResourceName, int& nUpID, int& nDownID, int& nOverID );
RESLIBRARY_API LRESULT GetBackGroundResourceID( LPCTSTR szResourceName, int& nBGID );

typedef struct _ButtonResource
{
	LPCTSTR	m_szBtnName;
	int		m_nBtnUpID;
	int		m_nBtnDownID;
	int		m_nBtnOverID;
} BTNRES;

typedef struct _BackGroundResource
{
	LPCTSTR m_szBGName;
	int		m_nBGID;
} BGRES;