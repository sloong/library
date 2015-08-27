/************************************************************************/
/*		ScriptParsing.h --- Script Parsing Classes Declaration			*/
/************************************************************************/
#pragma once

#ifdef SCRIPTPARSING_EXPORTS
#define SCRIPTPARSING_API __declspec(dllexport)
// Version
#define VERSION_NUMBER						2,2,0,11
#define VERSION_FILEVERSION					"2.2.0.11"
#define VERSION_BUILDTIME					"2014/2/18"
#ifdef _DEBUG
#define VERSION_PRODUCTVERSION 				"Ver.2.2 for Debug"
#define	VERSION_FILEDESCRIPTION				"Universal Script Parsing Debug Libaray"
#define	VERSION_PRODUCTNAME					"Universal Debug Libaray"
#else
#define VERSION_PRODUCTVERSION 				"Ver.2.2"
#define	VERSION_FILEDESCRIPTION				"Universal Script Parsing Libaray"
#define	VERSION_PRODUCTNAME					"Universal Libaray"
#endif // _DEBUG
#define VERSION_INTERNALNAME				"ScriptParsing.dll"
#define VERSION_COMPANYNAME 				"Sloong, Inc."
#define	VERSION_LEGALCOPYRIGHT				"Copyright (C) 2013 Sloong, Inc."
#else
#define SCRIPTPARSING_API __declspec(dllimport)
#endif

// Include Dependent Library Defines. the user must used it.
#include "CUniversal.h"
