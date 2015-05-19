#pragma once

#include "IUniversal.h"

using namespace Sloong::Universal;

class CLinkList : public ILinkList
{
public:
	CLinkList(void);

	//--- ~CLinkList Function annotation ---
	// Remarks:
	//		User cannot call it. and when list delete, this function don't call the destroy function. 
	~CLinkList(void);

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	//--- Locate Function annotation ---
	// Parameters:
	//		nIndex:
	//			A unsigned int value, it is the node index in this list.
	//		szMarkName:
	//			This node mark name, if have more than one, it always return the first node.
	//			So, don't use same mark to Insert the node. 
	// Return value:
	//		If function succeeds, return the node pointer, else return NULL.
	// Remarks:
	// 		Find node with the mark for index or name. this function have two, one is find with
	//		Index number, other one is find with mark name.
	CLinkList* Locate(UINT nIndex);
	CLinkList* Locate(LPCTSTR szMarkName);

	//--- GetLast Function annotation ---
	// Return value:
	//		A Pointer, point to the Last node.
	// Remarks:
	//		Get last node.
	CLinkList* GetLast();

	//--- Insert function annotation ---
	// Parameters:
	//		pData:		
	//			a void type pointer, it must used the new operator create in the Heap.
	//		szMarkName: 
	//			This data mark in this list, if have other node have same mark,
	//			we not sure when find time, it return which one, only when used the index number.
	//			but, we still don't recommend used the same mark in one list.
	//			This param default value is "UsdIndex". if user use index, user can use this value.
	//		szDataType: 
	//			this data type mark in this list, it have the default value, so it just 
	//			used help you this data isn't you want data.
	// Return Value:
	//		This node's index in list.
	// Remarks: 
	//		Insert the data to this node back, the param is the marks of the data.
	//		and the data must is a pointer with new memory.
	int Insert(LPVOID pData, LPCTSTR szMarkName = TEXT("UseIndex"), LPCTSTR szDataType = TEXT("LPVOID"));

	//--- AddToList Function annotation ---
	// Parameters:
	//		pData:
	//			A void type pointer, it must used the new operator create in the Heap.
	//		szMarkName:
	//			This data mark in this list, if have other node have same mark,
	//			we not sure when find time, it return which one, only when used the index number.
	//			but, we still don't recommend used the same mark in one list.
	//			This param default value is "UsdIndex". if user use index, user can use this value.
	//		szDataType:
	//			This data type mark, default is "void*".
	//	Return value:
	//		This node's index in list.
	//	Remarks:
	//		Insert node at the list last.
	int STDMETHODCALLTYPE Add(LPVOID pData, LPCTSTR szMarkName = TEXT("UseIndex"), LPCTSTR szDataType = TEXT("LPVOID"));

	//--- GetData Function annotation ---		
	// Return value:
	//		This node data, this is A void pointer, so you should translate it to your type, if you 
	//		don't know, see the member variable: m_szDataType.
	// Remarks:
	//		Get the data pointer, it return the void pointer, so you should change the type, if you don't know 
	//		it type, see the member variable: m_szDataType 
	LPVOID GetData();

	//--- SetData Function annotation ---
	// Parameters:
	//		pData:
	//			A void type pointer, it point to the new data as you want set.
	//		ulSize:
	//			Size of pData, LinkList will copy this data to list.
	//		bDelPrevData;
	//			If user was delete the data, the data pointer is invalid point. so 
	//			in this case, no delete the data.
	//	Remarks:
	//		Set node data.
	void SetData(LPVOID pData, bool bDelPrevData = true);

	//--- Delete Function annotation ---
	// Parameters:
	//		nIndex:
	//			Target node index number.
	// Remarks:
	//		Delete this node or other node in list, if no have param 1, it is delete this self.
	void Delete();
	void STDMETHODCALLTYPE Remove(int nIndex);

	//--- Destroy Function annotation ---
	// Remarks:
	//		Delete this list.
	void Destroy();

	ILinkList* STDMETHODCALLTYPE GetListHeader();
	int STDMETHODCALLTYPE Count();


private:
	// Set list last node, if the pLast is not a true node, maybe the list will chaos, so this 
	// function just can used in the self.
	void SetListLast(CLinkList* pLast);
	// Set the link list all node Numbers
	void SetListNum(UINT nNum);
	// Refresh the link list index, form head to last, refresh the index, 
	void RefreshIndex();


public:
	// Member variable.
	// Pointer to this list head node.
	CLinkList* m_pListHead;
	// Pointer to this list last node.
	CLinkList* m_pListLast;
	// Pointer to this node previous node.
	CLinkList* m_pPrevious;
	// Pointer to this node next node.
	CLinkList* m_pNext;
	// Name mark
	LPCTSTR m_szMarkName;
	// Pointer to the node data with the void,
	// if you want used is, please change the type
	void* m_pData;
	// the list all numbers.
	long m_nNum;
	// The index of this list.
	UINT m_nIndex;
	// The data type mark
	LPCTSTR m_szDataType;

	ULONG		m_Ref;
};