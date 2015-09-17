#ifdef _WINDOWS

#pragma once

class XMLProc
{
public:

	IXMLDOMDocumentPtr createFile;
	IXMLDOMDocumentPtr readFile;
	IXMLDOMElementPtr createRoot;
	IXMLDOMElementPtr readRoot;
	IXMLDOMProcessingInstructionPtr pPI;
	bool hr;

public:
	XMLProc();
	~XMLProc(){}
	IXMLDOMDocumentPtr Create(LPCTSTR rootTag);
	BOOL NewElement(LPCTSTR tag, LPCTSTR text, IXMLDOMElement** newNode);
	BOOL NewElement(LPCTSTR tag, IXMLDOMElement** newNode);
	BOOL AddChild(IXMLDOMElement* childElem, IXMLDOMElement* parentElem);
	BOOL SaveCreateFile(LPCTSTR dfile);
	IXMLDOMDocumentPtr Read(LPCTSTR sfile);
	BOOL SelectElement(LPCTSTR tag, IXMLDOMElement** node);
	BOOL GetText(IXMLDOMElement* elem, LPTSTR text, int nBufferSize, int* tlen);
	BOOL SaveReadFile(LPCTSTR dfile);
	BOOL ChangeText(IXMLDOMElement* elem, LPCTSTR text);
	BOOL GetParent(IXMLDOMElement* childElem, IXMLDOMElement** parentElem);
	BOOL GetChild(IXMLDOMElement** childElem, IXMLDOMElement* parentElem);
	BOOL RemoveChild(IXMLDOMElement* childElem, IXMLDOMElement* parentElem);
	BOOL SetAttribute(IXMLDOMElement* node, LPCTSTR attr, LPCTSTR value);
	IXMLDOMDocumentPtr GetCreateFile();
	IXMLDOMDocumentPtr GetReadFile();
	IXMLDOMElement* GetCreateRoot();
	IXMLDOMElement* GetReadRoot();

	void Fail();

	void Exit();

};
#endif // _WINDOWS
