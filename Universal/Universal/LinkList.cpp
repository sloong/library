/************************************************************************/
/*		 LinkList.cpp --- Current Link List Library Implement			*/
/************************************************************************/
//--- 2013/06/07 --- WCB --- Add
#include "StdAfx.h"
#include "SLUniversal.h"

using namespace SoaringLoong;

//--- 2013/06/07 --- WCB --- Add
//* Remarks:
//*		Init all value .
CLinkList::CLinkList(void)
{
	m_nIndex = 0;
	m_nNum = 0;
	m_pNext = NULL;
	m_pPrevious = NULL;
	m_pData = NULL;
	m_pListHead = NULL;
	m_pListLast = NULL;
	//--- 2013/06/09 --- WCB --- Add init value .
	m_szDataType = NULL;
	m_szMarkName = NULL;
}


//--- 2013/06/07 --- WCB --- Add
//--- 2013/7/5 --- WCB --- Modify
//* Remarks: 
//*		Not Call the Destroy function to delete the list. Because in Destroy function, it will use Delete() function to delete the temp node, 
//*		but the temp node is a CLinkList type value, so, it will call this function again, but, the memory was freed, delete it
//*		again will be failed. so ,not call it. if user want delete the list, call the Destroy.
CLinkList::~CLinkList(void)
{
	//Delete();
}


//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Insert the data to this node back, the param is the marks of the data.
//*		and the data must is a pointer with new memory. in many time, you should used
//*		the AddtoList() function ,no this.
//--- 0000031 --- 2013/12/4 --- WCB --- Modify
// Delete the pData check , allow pData is NULL.
int CLinkList::Insert( LPVOID pData, LPCTSTR szMarkName /* = TEXT("UseIndex") */, LPCTSTR szDataType /* = TEXT("LPVOID") */ )
{
	//--- 0000044 --- 2013/12/5 --- WCB --- Add
	//--- 0000049 --- 2013/12/10 --- WCB --- Modify
	LPTSTR szNewName = NULL;
	LPTSTR szNewType = NULL;
	if ( NULL != szMarkName )
	{
		szNewName = new TCHAR[_tcslen( szMarkName )+1];
		_tcscpy_s( szNewName, _tcslen(szMarkName)+1, szMarkName );
	}
	if ( NULL != szDataType )
	{
		szNewType = new TCHAR[_tcslen( szDataType )+1];
		_tcscpy_s( szNewType, _tcslen(szDataType)+1, szDataType );
	}
	//if the link list is empty, do it.
	//--- 0000045 --- 2013/12/6 --- WCB --- Modify ---
	// Modify List check.
	if ( NULL == m_pListHead && NULL == m_pListLast )
	{
		m_pData = pData;
		m_szMarkName = szNewName;
		m_pListHead = this;
		m_pListLast = this;
		m_nNum = 1;
		m_szDataType = szNewType;
		m_nIndex = 0;
		return m_nIndex;
	}
	//if this is the link list last node, do it
	if ( NULL == m_pNext )
	{
		//Create the next node
		m_pNext = new CLinkList;
		// Set the new node value.
		m_pNext->m_nIndex = m_nIndex + 1;
		m_pNext->m_pPrevious = this;
		m_pNext->m_pNext = NULL;
		m_pNext->m_szMarkName = szNewName;
		m_pNext->m_pData = pData;
		m_pNext->m_pListHead = m_pListHead;
		m_pNext->m_pListLast = m_pNext;
		m_pNext->m_szDataType = szNewType;

		SetListLast(m_pNext);
		SetListNum(m_pListLast->m_nIndex+1);
		return m_pNext->m_nIndex;
	}
	//if not insert to last, do it.
	else
	{
		CLinkList* pTmp = new CLinkList;
		pTmp->m_pNext = this->m_pNext;
		pTmp->m_pPrevious = this;
		pTmp->m_pData = pData;
		pTmp->m_szMarkName = szNewName;
		this->m_pNext->m_pPrevious = pTmp;
		this->m_pNext = pTmp;
		m_pNext->m_pListHead = m_pListHead;
		m_pNext->m_pListLast = m_pListLast;
		m_pNext->m_szDataType = szNewType;
		//change all node index value.
		int nIndex = this->m_nIndex;
		nIndex = this->m_nIndex;
		while( NULL != pTmp->m_pNext )
		{
			pTmp->m_nIndex = nIndex ++;
			pTmp = pTmp->m_pNext;
		}
		SetListNum(m_pListLast->m_nIndex+1);
		return pTmp->m_nIndex;
	}
}

//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Find the node whit the index number in this list.
CLinkList* CLinkList::Locate( UINT nIndex )
{
	//--- 0000006 --- 2013/9/26 --- WCB --- Add
	// Before search, check the list is not a empty list.
	if ( NULL == m_pListHead )
	{
		return NULL;
	}

	if( m_nIndex == nIndex )
	{
		return this;
	}
	else if ( m_nIndex > nIndex )
	{
		CLinkList* pTmp = m_pPrevious;
		while( NULL != pTmp )
		{
			if ( nIndex == pTmp->m_nIndex )
			{
				return pTmp;
			}
			else
			{
				pTmp = pTmp->m_pPrevious;
			}
		}
		return NULL;
	}
	else if ( m_nIndex < nIndex)
	{
		CLinkList* pTmp = m_pNext;
		while( NULL != pTmp )
		{
			if ( nIndex == pTmp->m_nIndex )
			{
				return pTmp;
			}
			else
			{
				pTmp = pTmp->m_pNext;
			}
		}
		return NULL;
	}
	else
	{
		return NULL;
	}
}


//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Get this node data pointer.
LPVOID CLinkList::GetData()
{
	//--- WCB --- 2013/7/18 --- Modify ---
	return m_pData;
}

//--- 0000030 --- 2013/12/4 --- WCB --- Add
// Remarks:
//		Set node data.
void CLinkList::SetData( LPVOID pData, bool bDelPrevData /* = true */ )
{
	if ( bDelPrevData )
	{
		SAFE_DELETE_ARR( m_pData );
	}
	LPVOID pNewData = NULL;
	//--- 0000049 --- 2013/12/10 --- WCB --- Modify
	m_pData = pNewData;
}

//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Get the last node pointer from this list.
CLinkList* CLinkList::GetLast()
{
	return m_pListLast;
}

//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Destroy all resource and data in this list
void CLinkList::Destroy()
{
	// Get list last node
	CLinkList* pTmp = m_pListLast;

	// I think, when this while done, the list all node is be release.
	while( NULL != pTmp )
	{
		SAFE_DELETE_ARR( pTmp->m_pData );
		CLinkList* pPrevTmp = pTmp->m_pPrevious;
		pTmp->m_pListHead = NULL;
		pTmp->m_pListLast = NULL;
		pTmp->m_pNext = NULL;
		pTmp->m_pPrevious = NULL;
		SAFE_DELETE_ARR( pTmp->m_szDataType );
		SAFE_DELETE_ARR( pTmp->m_szMarkName );
		// No delete the caller.
		if ( m_nIndex != pTmp->m_nIndex )
		{
			SAFE_DELETE(pTmp);
		}
		pTmp = NULL;
		pTmp = pPrevTmp;
	}

	// In here, it all variable should be NULL.
	m_pListHead = NULL;
	m_pListLast = NULL;
	m_pNext = NULL;
	m_pPrevious = NULL;
	SAFE_DELETE_ARR( m_pData );
	SAFE_DELETE_ARR( m_szDataType );
	SAFE_DELETE_ARR( m_szMarkName );
	m_nIndex = 0;
	m_nNum = 0;
}

//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Find the node with the mark name in this list.
CLinkList* CLinkList::Locate( LPCTSTR szMarkName )
{
	CLinkList* pTmp = Locate( (UINT)0 );
	
	while( NULL != pTmp )
	{
		if ( 0 == _tcscmp(pTmp->m_szMarkName,szMarkName) )
		{
			return pTmp;
		}
		else
		{
			pTmp = pTmp->m_pNext;
		}
	}
	return NULL;
}


//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Set all node last pointer value, it just set all node Last node value, not change other value.
//*		if the pLast is not a true node, it can may be the list
//*		chaos, so , use this function need check the node value isn't all true.
void CLinkList::SetListLast(CLinkList* pLast)
{
	CLinkList* pTmp = m_pListHead;
	while( NULL != pTmp )
	{
		pTmp->m_pListLast = pLast;
		pTmp = pTmp->m_pNext;
	}
}


//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Insert a node in the list last, many time you should used this function not the Insert() function .
int CLinkList::AddToList( LPVOID pData, LPCTSTR szMarkName /* = TEXT("UseIndex") */, LPCTSTR szDataType /* = TEXT("LPVOID") */ )
{
	// empty list
	int nIndex = 0;
	if ( NULL == m_pListHead && NULL == m_pListLast )
	{
		//--- 2013/06/09 --- WCB --- Add Data Type to list.
		nIndex = this->Insert( pData, szMarkName, szDataType );
	}
	else
	{
		//--- 2013/06/09 --- WCB --- Add Data Type to list.
		nIndex = m_pListLast->Insert( pData, szMarkName, szDataType );
	}
	return nIndex;
}

//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Set all node Numbers mark, if the num is not true , it will make the list chaos.
void CLinkList::SetListNum( UINT nNum )
{
	CLinkList* pTmp = m_pListHead;
	while( NULL != pTmp )
	{
		pTmp->m_nNum = nNum;
		pTmp = pTmp->m_pNext;
	}
}


//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Delete this node from this list.
void CLinkList::Delete()
{
	//check the link list is not empty
	if ( NULL == m_pListHead || NULL == m_pListLast )
	{
		Destroy();
	}
	else
	{
		//if delete the link list head
		if ( NULL == m_pPrevious )
		{
			if ( NULL == m_pNext )
			{
				// Just have one node.
				Destroy();
				return;
			}
			m_pNext->m_pPrevious = NULL;
		}
		//else if delete the link list last node
		else if ( NULL == m_pNext )
		{
			m_pPrevious->m_pNext = NULL;
		}
		else
		{
			m_pNext->m_pPrevious = m_pNext;
			m_pPrevious->m_pNext = m_pPrevious;
		}
		//change the index and Numbers.
		RefreshIndex();
		SetListNum(m_nNum-1);
		SAFE_DELETE_ARR(m_pData);
		SAFE_DELETE_ARR( m_szDataType );
		SAFE_DELETE_ARR( m_szMarkName );
	}
	delete this;
}

//--- 2013/7/2 --- WCB --- Add
// Remarks:
//		Delete the appointed node with index number
void CLinkList::Delete( int nIndex )
{
	// Checking value
	if ( nIndex < 0 || nIndex > m_nNum )
	{
		return;
	}

	// Get delete node pointer.
	CLinkList* pNode = NULL;
	pNode = Locate( nIndex );
	if ( NULL == pNode )
	{
		return;
	}

	// Delete 
	pNode->Delete();
}


//--- 2013/06/07 --- WCB --- Add
//* Remarks: 
//*		Refresh the index numbers, it will set the all node index from the list head.
void CLinkList::RefreshIndex()
{
	CLinkList* pTmp = m_pListHead;
	int nIndex = 0;
	while( NULL != pTmp )
	{
		pTmp->m_nIndex = nIndex;
		nIndex++;
		pTmp = pTmp->m_pNext;
	}
}