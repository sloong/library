/************************************************************************/
/*			 XMLParser.h --- XML Parsing Class Declaration				*/
/************************************************************************/
#ifndef XMLPARSER_H
#define XMLPARSER_H



#define CHAR_SWAP_SIZE	2048

using namespace Sloong::Universal;

class TiXmlDocument;
class TiXmlElement;
class TiXmlAttribute;

#include "univ.h"
typedef map<wstring, LPVOID> ILinkList;

class CXMLParser
{
public:
	TiXmlDocument*	m_pDoc;
	TiXmlElement*	m_pRootNode;

public:
	CXMLParser(void);
	~CXMLParser(void);

	LPCTSTR GetAttribute(LPCTSTR szNodeName, LPCTSTR szAttributeName, bool& hRes, LPCTSTR szParentNode = NULL, bool bFindParent = false);
	LPCTSTR GetNodeText(LPCTSTR szNodeName, bool& hRes, LPCTSTR szParentNode = NULL);
	int GetAttributeInt(LPCTSTR szNodeName, LPCTSTR szAttributeName, bool& hRes, LPCTSTR szParientNode = NULL, bool bFindParent = false);
	ULONG GetAttributeARGB(LPCTSTR szNodeName, bool& hRes, LPCTSTR szParientNode = NULL, bool bFindParent = false);
	bool FindAllChildName(LPCTSTR szParentNodeName, ILinkList pChildList, LPCTSTR szRootNodeName = NULL);
	bool FindAllChildText(LPCTSTR szParentNodeName, ILinkList pChildList, LPCTSTR szRootNodeName = NULL);
	bool Initialize(LPCTSTR strPathe);
	bool GetNodeByName(TiXmlElement* pRoot, LPCTSTR strNodeName, TiXmlElement*& pNode);
	bool FindAllChildName(TiXmlElement* pNode, ILinkList pList);
	bool FindAllChildText(TiXmlElement* pNode, ILinkList pList);
	void Shutdown();
	LPCTSTR GetAttribute(TiXmlAttribute* pNodeAttribute, LPCTSTR strAttributeName, bool& hRes);
	LPCTSTR GetAttribute(TiXmlElement* pNode, LPCTSTR strAttributeName, bool& hRes);
	LPCTSTR GetNodeText(TiXmlElement* pNode, bool& hRes);
	bool GetParentNode(TiXmlElement* pChildNode, TiXmlElement*& pParentNode);
	bool g_hRes;
};
#endif // !XMLPARSER_H
