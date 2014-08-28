/************************************************************************/
/*			 XMLParser.h --- XML Parsing Class Declaration				*/
/************************************************************************/
#pragma once

#define CHAR_SWAP_SIZE	2048

class TiXmlDocument;
class TiXmlElement;
class TiXmlAttribute;

class CXMLParser
{
public:
	TiXmlDocument*	m_pDoc;
	TiXmlElement*	m_pRootNode;

public:
	CXMLParser(void);
	~CXMLParser(void);

	LRESULT Initialize( LPCTSTR strPathe);
	LRESULT GetNodeByName(TiXmlElement* pRoot, LPCTSTR strNodeName, TiXmlElement*& pNode);
	LRESULT FindAllChildName( TiXmlElement* pNode, CLinkList*& pList );
	LRESULT FindAllChildText( TiXmlElement* pNode, CLinkList*& pList );
	void Shutdown();
	LPCTSTR GetAttribute( TiXmlAttribute* pNodeAttribute, LPCTSTR strAttributeName ,LRESULT& lResult);
	LPCTSTR GetAttribute( TiXmlElement* pNode, LPCTSTR strAttributeName, LRESULT& lResult );
	LPCTSTR GetNodeText( TiXmlElement* pNode, LRESULT& lResult );
	LRESULT GetParentNode( TiXmlElement* pChildNode, TiXmlElement*& pParentNode );
};