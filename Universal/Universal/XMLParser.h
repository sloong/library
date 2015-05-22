/************************************************************************/
/*			 XMLParser.h --- XML Parsing Class Declaration				*/
/************************************************************************/
#pragma once

#define CHAR_SWAP_SIZE	2048

using namespace Sloong::Universal;

class TiXmlDocument;
class TiXmlElement;
class TiXmlAttribute;

#include "SloongUniversal.h"
typedef map<wstring,LPVOID> ILinkList;

class UNIVERSAL_API CXMLParser
{
public:
	TiXmlDocument*	m_pDoc;
	TiXmlElement*	m_pRootNode;

public:
	CXMLParser(void);
	~CXMLParser(void);

	LPCTSTR GetAttribute(LPCTSTR szNodeName, LPCTSTR szAttributeName, HRESULT& hRes, LPCTSTR szParentNode = NULL, bool bFindParent = false);
	LPCTSTR GetNodeText(LPCTSTR szNodeName, HRESULT& hRes, LPCTSTR szParentNode = NULL);
	int GetAttributeInt(LPCTSTR szNodeName, LPCTSTR szAttributeName, HRESULT& hRes, LPCTSTR szParientNode = NULL, bool bFindParent = false);
	ULONG GetAttributeARGB(LPCTSTR szNodeName, HRESULT& hRes, LPCTSTR szParientNode = NULL, bool bFindParent = false);
	HRESULT FindAllChildName(LPCTSTR szParentNodeName, ILinkList pChildList, LPCTSTR szRootNodeName = NULL);
	HRESULT FindAllChildText(LPCTSTR szParentNodeName, ILinkList pChildList, LPCTSTR szRootNodeName = NULL);
	HRESULT Initialize(LPCTSTR strPathe);
	HRESULT GetNodeByName(TiXmlElement* pRoot, LPCTSTR strNodeName, TiXmlElement*& pNode);
	HRESULT FindAllChildName(TiXmlElement* pNode, ILinkList pList);
	HRESULT FindAllChildText(TiXmlElement* pNode, ILinkList pList);
	void Shutdown();
	LPCTSTR GetAttribute(TiXmlAttribute* pNodeAttribute, LPCTSTR strAttributeName, HRESULT& hRes);
	LPCTSTR GetAttribute(TiXmlElement* pNode, LPCTSTR strAttributeName, HRESULT& hRes);
	LPCTSTR GetNodeText(TiXmlElement* pNode, HRESULT& hRes);
	HRESULT GetParentNode(TiXmlElement* pChildNode, TiXmlElement*& pParentNode);
	HRESULT g_hRes;
};