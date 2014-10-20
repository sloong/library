/************************************************************************/
/*				XML.cpp --- XML Parsing Classes Implement				*/
/************************************************************************/
//--- 2013/7/5 --- WCB --- Add
#include "StdAfx.h"
#include "IUniversal.h"
#include "SLoongError.h"
#include "Universal/TinyXML/tinyxml.h"
#include "XMLParser.h"

using namespace SoaringLoong;

//--- 2013/7/5 --- WCB --- Add
CXMLParser::CXMLParser(void)
{

}

//--- 2013/7/5 --- WCB --- Add
CXMLParser::~CXMLParser(void)
{
}


//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get Node Text Value.
LPCTSTR CXMLParser::GetNodeText( LPCTSTR szNodeName, HRESULT& hRes, LPCTSTR szParentNode /* = NULL */ )
{
	TiXmlElement* pNode = NULL;
	TiXmlElement* pParentNode = NULL;
	if ( NULL != szParentNode )
	{
		if( S_OK != GetNodeByName( m_pRootNode,szParentNode,pParentNode ))
		{
			hRes = S_FALSE;
			return NULL;
		}
	}
	else
	{
		pParentNode = m_pRootNode;
	}
	g_hRes = GetNodeByName( pParentNode, szNodeName, pNode );
	if( S_OK != g_hRes )
	{
		hRes = S_FALSE;
		return NULL;
	}

	LPCTSTR pTmp = GetNodeText( pNode, g_hRes );
	if ( NULL == pTmp && S_FALSE == g_hRes)
	{
		return NULL;
	}
	else
	{
		return pTmp;
	}
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get node by name.
HRESULT CXMLParser::GetNodeByName(TiXmlElement* pRootElement, LPCTSTR strNodeName, TiXmlElement*& pNode)
{ 
	if( NULL == pRootElement )
		return S_FALSE;

	if ( 0 == _tcscmp( strNodeName, pRootElement->Value() ) )  
	{  
		pNode = pRootElement;  
		return S_OK;  
	}  
	TiXmlElement* pTmpEle = pRootElement;    
	for (pTmpEle = pRootElement->FirstChildElement(); pTmpEle; pTmpEle = pTmpEle->NextSiblingElement())    
	{    
		//call self to find the node
		if( S_OK == GetNodeByName(pTmpEle,strNodeName,pNode))  
			return S_OK;  
	}    
	return S_FALSE;  
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get node Attribute
LPCTSTR CXMLParser::GetAttribute(TiXmlAttribute* pNodeAttribute, LPCTSTR strAttributeName ,HRESULT& hRes)
{
	
	for( ; pNodeAttribute; pNodeAttribute = pNodeAttribute->Next() )
	{
		if ( 0 == _tcscmp( strAttributeName, pNodeAttribute->Name()) )
		{
			hRes = S_OK;
			return pNodeAttribute->Value();
		}
	}
	hRes = S_FALSE;
	return TEXT("");
}


//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get Node Attribute value, param two is attribute name,
LPCTSTR CXMLParser::GetAttribute( LPCTSTR szNodeName, LPCTSTR szAttributeName, HRESULT& hRes, LPCTSTR szParentNode /* = NULL */, bool bFindParent /* = false */ )
{
	TiXmlAttribute* pNodeAttribute = NULL;
	TiXmlElement* pNode = NULL;
	TiXmlElement* pParentNode = NULL;
	if ( NULL == szParentNode )
	{
		pParentNode = m_pRootNode;
	}
	else
	{
		if ( S_OK != GetNodeByName(m_pRootNode,szParentNode,pParentNode))
		{
			hRes = S_FALSE;
			return NULL;
		}
		if ( NULL == pParentNode)
		{
			hRes = S_FALSE;
			return NULL;
		}
	}
	g_hRes = GetNodeByName(pParentNode,szNodeName,pNode);
	if ( S_OK != g_hRes )
	{
		hRes = S_FALSE;
		return NULL;
	}

	// if no find the attribute, get the result, and return the value
	LPCTSTR szValue = GetAttribute( pNode, szAttributeName, hRes );
	if ( S_OK == hRes)
	{
		// find, return result.
		return szValue;
	}
	else
	{
		// not find, check mark 
		if ( true == bFindParent )
		{
			HRESULT lFindRes = S_OK;
			// Get Parent , find again.
			for ( lFindRes = GetParentNode( pNode, pNode ); lFindRes == S_OK; lFindRes = GetParentNode( pNode, pNode ) )
			{
				szValue = GetAttribute( pNode, szAttributeName, hRes );
				if ( S_OK == hRes )
				{
					return szValue;
				}
			}
			// Not find, return
			hRes = S_FALSE;
			return NULL;
		}
		else
		{
			hRes = S_FALSE;
			return NULL;
		}
	}
}


//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get Node Attribute value, it return value type is int , make sure the attribute value is int.
//--- WCB --- 2013/7/18 --- Modify ---
int CXMLParser::GetAttributeInt( LPCTSTR szNodeName, LPCTSTR szAttributeName , HRESULT& hRes , LPCTSTR szParientNode /* = NULL */, bool bFindParent /* = false */ )
{
	if ( NULL == szNodeName || NULL == szAttributeName )
	{
		hRes = S_FALSE;
		return 0;
	}
	LPCTSTR szValue = GetAttribute(szNodeName,szAttributeName,g_hRes,szParientNode,bFindParent);
	if ( NULL == szValue )
	{
		hRes = S_FALSE;
		return 0;
	}
	return _ttoi(szValue);
}


//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Find in a node all child node name, result save in a link list.
//	The List is a new object and the List head is null. so it is a empty list.
//	The Root node name is the find Starting point, if is NULL, find from the root.
HRESULT CXMLParser::FindAllChildName( LPCTSTR szParentNodeName, ILinkList* pChildList, LPCTSTR szRootNodeName /* = NULL */ )
{
	TiXmlElement* pNode = NULL;
	TiXmlElement* pRootNode = NULL;
	if ( NULL != szRootNodeName )
	{
		g_hRes = GetNodeByName( m_pRootNode, szRootNodeName, pRootNode);
	}
	else
	{
		pRootNode = m_pRootNode;
	}

	if ( S_OK != g_hRes )
	{
		return g_hRes;
	}
	g_hRes = GetNodeByName( pRootNode, szParentNodeName, pNode );
	if ( S_OK != g_hRes )
	{
		return g_hRes;
	}
	g_hRes = FindAllChildName( pNode, pChildList );
	if ( S_OK != g_hRes )
	{
		return g_hRes;
	}
	return S_OK;
}


//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get Node Attribute value, it return the ARGB value, it's a unsigned long value, so make sure the attribute value is ARGB .
//--- WCB --- 2013/7/18 --- Modify ---
ULONG CXMLParser::GetAttributeARGB( LPCTSTR szNodeName, HRESULT& hRes , LPCTSTR szParientNode /* = NULL */, bool bFindParent /* = false */ )
{
	if ( NULL == szNodeName )
	{
		hRes = S_FALSE;
		return 0;
	}
	int a = GetAttributeInt(szNodeName,TEXT("ColorA"),g_hRes,szParientNode,bFindParent);
	if ( S_OK != g_hRes )
	{
		hRes = g_hRes;
		return 0;
	}
	int r = GetAttributeInt(szNodeName,TEXT("ColorR"),g_hRes,szParientNode,bFindParent);
	if ( S_OK != g_hRes )
	{
		hRes = g_hRes;
		return 0;
	}
	int g = GetAttributeInt(szNodeName,TEXT("ColorG"),g_hRes,szParientNode,bFindParent);
	if ( S_OK != g_hRes )
	{
		hRes = g_hRes;
		return 0;
	}
	int b = GetAttributeInt(szNodeName,TEXT("ColorB"),g_hRes,szParientNode,bFindParent);
	if ( S_OK != g_hRes )
	{
		hRes = g_hRes;
		return 0;
	}
	return _ARGB(a,r,g,b);
}



//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get the node all child node text value,
HRESULT CXMLParser::FindAllChildText(LPCTSTR szParentNodeName, ILinkList* pChildList, LPCTSTR szRootNodeName /* = NULL */)
{
	TiXmlElement* pNode = NULL;
	TiXmlElement* pRootNode = NULL;
	if ( NULL != szRootNodeName )
	{
		g_hRes = GetNodeByName( m_pRootNode, szRootNodeName, pRootNode);
	}
	else
	{
		pRootNode = m_pRootNode;
	}

	if ( S_OK != g_hRes )
	{
		return g_hRes;
	}
	g_hRes = GetNodeByName( pRootNode, szParentNodeName, pNode );
	if ( S_OK != g_hRes )
	{
		return g_hRes;
	}
	g_hRes = FindAllChildText( pNode, pChildList );
	if ( S_OK != g_hRes )
	{
		return g_hRes;
	}
	return S_OK;
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get Attribute by Node.
LPCTSTR CXMLParser::GetAttribute( TiXmlElement* pNode, LPCTSTR strAttributeName, HRESULT& hRes )
{
	if ( NULL == pNode || 0 == _tcscmp(TEXT(""),strAttributeName) )
	{
		hRes = S_FALSE;
		return NULL;
	}

	TiXmlAttribute* pNodeAttribute = NULL;
	pNodeAttribute = pNode->FirstAttribute();
	if ( NULL == pNodeAttribute )
	{
		hRes = S_FALSE;
		return NULL;
	}
	TiXmlNode* pTest = pNode->Parent();
	TiXmlElement* pTestElm = pTest->ToElement();
	return GetAttribute( pNodeAttribute, strAttributeName, hRes );
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get node Parent
HRESULT CXMLParser::GetParentNode( TiXmlElement* pChildNode, TiXmlElement*& pParentNode )
{
	if ( NULL == pChildNode )
	{
		return S_FALSE;
	}

	// Child node is root node. return false
	if ( pChildNode == m_pRootNode )
	{
		pParentNode = m_pRootNode;
		return S_OK;
	}

	TiXmlNode* pParent = pChildNode->Parent();
	if ( NULL == pParent )
	{
		return S_ERROR_FIND_OBJECT;
	}
	pParentNode = pParent->ToElement();
	if ( NULL == pParentNode )
	{
		return S_ERROR_FIND_OBJECT;
	}
	else
	{
		return S_OK;
	}
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Initialization the XML parsing.
HRESULT CXMLParser::Initialize( LPCTSTR strPath )
{
	if( 0 == _tcscmp( TEXT("") ,strPath))
		return S_ERROR_PARAM_VALUE;

	m_pDoc = new TiXmlDocument();

// 	FILE* pFile;
// 	if ( 0 != _tfopen_s( &pFile, strPath, TEXT("a+") ) )
// 	{
// 		return -2;
// 	}
	 
	if ( false == m_pDoc->LoadFile( (strPath) ))
	{
		return S_ERROR_LOAD_SCRIPT_FILE;
	}

	m_pRootNode = m_pDoc->RootElement();
	if ( NULL == m_pRootNode )
	{
		return S_FALSE;
	}
	return S_OK;
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get Node Name, search result is save in MarkName.
HRESULT CXMLParser::FindAllChildName(TiXmlElement* pNode, ILinkList* pList)
{
	// check the into value
	if ( NULL == pNode || NULL != pList->GetListHeader() )
	{
		return S_ERROR_PARAM_VALUE;
	}

	// Get the First Child
	TiXmlElement* pFirst = pNode->FirstChildElement();
	if ( NULL == pFirst )
	{
		return S_ERROR_FIND_OBJECT;
	}

	LPCTSTR str = pFirst->Value();
	if ( NULL == str )
	{
		return S_ERROR_FIND_OBJECT;
	}

	pList->Add( NULL, str, TEXT("LPCTSTR") );
	
	// Get other node
	TiXmlElement* pTmp = pFirst->NextSiblingElement();
	
	// start
	while(1)
	{
		if ( NULL != pTmp )
		{
			// add to list
			str = pTmp->Value();
			pList->Add( NULL, str, TEXT("LPCTSTR") );
			// Get next node
			pTmp = pTmp->NextSiblingElement();
		}
		else
		{
			// is last node,return 
			return S_OK;
		}
	}
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Get Node text value
HRESULT CXMLParser::FindAllChildText(TiXmlElement* pNode, ILinkList* pList)
{
	// check the into value
	if ( NULL == pNode || NULL != pList->GetListHeader() )
	{
		return S_ERROR_PARAM_VALUE;
	}

	// Get the First Child
	TiXmlElement* pFirst = pNode->FirstChildElement();
	if ( NULL == pFirst )
	{
		return S_ERROR_FIND_OBJECT;
	}

	LPCTSTR str = pFirst->GetText();
	if ( NULL == str )
	{
		return S_ERROR_FIND_OBJECT;
	}

	pList->Add( NULL, str, TEXT("LPCTSTR"));
	
	// Get other node
	TiXmlElement* pTmp = pFirst->NextSiblingElement();
	// start
	while(1)
	{
		if ( NULL != pTmp )
		{
			// add to list
			str = pTmp->GetText();
			pList->Add( NULL, str, TEXT("LPCTSTR"));
			// Get next node
			pTmp = pTmp->NextSiblingElement();
		}
		else
		{
			// is last node,return 
			return S_OK;
		}
	}
}

//--- 2013/7/5 --- WCB --- Add
// Remarks:
//		Release all memory resource
void CXMLParser::Shutdown()
{
	m_pDoc->Clear();
	//--- WCB --- 2013/7/18 --- Add ---
	if ( NULL != m_pDoc )
	{
		m_pDoc->Clear();
		SAFE_DELETE( m_pDoc );
	}
	delete this;
}

//--- 2013/7/5 --- WCB --- Add ---
// Remarks:
//		Get Node Text, if function failed, it return NULL, and the hRes is S_FALSE.
LPCTSTR CXMLParser::GetNodeText( TiXmlElement* pNode, HRESULT& hRes )
{
	if ( NULL == pNode )
	{
		hRes = S_FALSE;
		return NULL;
	}

	return (pNode->GetText());
}