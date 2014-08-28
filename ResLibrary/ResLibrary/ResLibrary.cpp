/************************************************************************/
/*		ResLibrary.cpp --- Defines the Resource Library List			*/
/************************************************************************/
#include "stdafx.h"

#include "LinkList.h"
#include "resource.h"

#include "ResLibrary.h"

#pragma comment(lib,"LinkList.lib")

// This is an example of an exported variable
RESLIBRARY_API CLinkList* g_pButtonResList = NULL;
RESLIBRARY_API CLinkList* g_pBackGroundResList = NULL;


//--- 2013/7/4 --- WCB --- Add
// Remarks:
//		Initialize the Resource Library List.
extern LRESULT Initialize()
{
	if( NULL != g_pButtonResList || NULL != g_pBackGroundResList )
	{
		Shutdown();
	}

	g_pButtonResList = new CLinkList;
	g_pBackGroundResList = new CLinkList;

	BTNRES ButtonRes[] = 
	{
		{ TEXT("Back"),		IDB_BACK_UP,	IDB_BACK_DOWN,		IDB_BACK_OVER		},
		{ TEXT("Credits"),	IDB_CREDITS_UP,	IDB_CREDITS_DOWN,	IDB_CREDITS_OVER	},
		{ TEXT("Level1"),	IDB_LEVEL1_UP,	IDB_LEVEL1_DOWN,	IDB_LEVEL1_OVER		},
		{ TEXT("Quit"),		IDB_QUIT_UP,	IDB_QUIT_DOWN,		IDB_QUIT_OVER		},
		{ TEXT("Start"),	IDB_START_UP,	IDB_START_DOWN,		IDB_START_OVER		}
	};

	BGRES BackGroundRes[] = 
	{
		{ TEXT("Credits"),	IDB_CREDITS_MENU	},
		{ TEXT("Main"),		IDB_MAIN_MENU		},
		{ TEXT("Loading"),	IDB_LOADING_MENU	},
		{ TEXT("Start"),	IDB_START_MENU		}
	};

	int nBtnNum = sizeof(ButtonRes) / sizeof(BTNRES);
	for ( int i = 0; i < nBtnNum; i++ )
	{
		LPINT pBtnID = new int[3];
		pBtnID[0] = ButtonRes[i].m_nBtnUpID;
		pBtnID[1] = ButtonRes[i].m_nBtnDownID;
		pBtnID[2] = ButtonRes[i].m_nBtnOverID;

		g_pButtonResList->AddToList( pBtnID, ButtonRes[i].m_szBtnName, TEXT("int[3]"));
	}

	int nBGNum = sizeof( BackGroundRes ) / sizeof( BGRES );
	for (int i = 0; i < nBGNum; i++)
	{
		LPINT pBGID = new int;
		*pBGID = BackGroundRes[i].m_nBGID;

		g_pBackGroundResList->AddToList( pBGID, BackGroundRes[i].m_szBGName, TEXT("int") );
	}
	return S_OK;
}

//--- 2013/7/4 --- WCB --- Add
// Remarks:
//		Delete all memory resource 
extern void Shutdown()
{
	SAFE_DESTROY( g_pBackGroundResList );
	SAFE_DESTROY( g_pButtonResList );
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get Button Resource ID with Button Name.
RESLIBRARY_API LRESULT GetButtonResourceID( LPCTSTR szResourceName , int& nUpID, int& nDownID, int& nOverID )
{
	if ( NULL == g_pButtonResList )
	{
		if( S_FALSE == Initialize())
			return S_FALSE;
	}

	CLinkList* pNode = NULL;
	pNode = g_pButtonResList->Locate( szResourceName );
	if ( NULL == pNode )
	{
		return S_FALSE;
	}

	int* pData = NULL;
	pData = (int*)pNode->GetData();
	if ( NULL == pData )
	{
		return S_FALSE;
	}

	nUpID = pData[0];
	nDownID = pData[1];
	nOverID = pData[2];

	return S_OK;
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get BackGround Resource ID with BackGround name
RESLIBRARY_API LRESULT GetBackGroundResourceID( LPCTSTR szResourceName, int& nBGID )
{
	if ( NULL == g_pBackGroundResList )
	{
		if ( S_FALSE == Initialize() )
		{
			return S_FALSE;
		}
	}

	CLinkList* pNode = NULL;
	//--- WCB --- 2013/7/18 --- Modify ---
	pNode = g_pBackGroundResList->Locate( szResourceName );
	if ( NULL == pNode )
	{
		return S_FALSE;
	}

	LPINT pData = NULL;
	pData = (LPINT)pNode->GetData();
	if ( NULL == pData )
	{
		return S_FALSE;
	}

	nBGID = pData[0];

	return S_OK;
}