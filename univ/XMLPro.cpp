 #include "stdafx.h"
// #include "XMLPro.h"
// XMLProc::XMLProc()
// 
// {
// 
// 	//CoInitialize(NULL);
// 
// 	createFile = NULL;
// 
// 	readFile = NULL;
// 
// 	createRoot = NULL;
// 
// 	readRoot = NULL;
// 
// 	pPI = NULL;
// 
// }
// 
// IXMLDOMDocumentPtr XMLProc::Create(LPCTSTR rootTag)
// 
// {
// 
// 	if (rootTag == NULL)
// 
// 		return 0;
// 
// 	hr = createFile.CreateInstance(__uuidof(DOMDocument));
// 
// 	assert(hr);
// 
// 	hr = createFile->createProcessingInstruction(_bstr_t(_T("xml")), _bstr_t(_T("version=\"1.0\" encoding=\"GBK\"")), &pPI);
// 
// 	//hr = createFile->createProcessingInstruction( _bstr_t(_T("xml")), _bstr_t(_T("version=\"1.0\"")), &pPI );
// 
// 	assert(hr);
// 
// 	hr = createFile->appendChild(pPI, NULL);
// 
// 	assert(hr);
// 
// 	hr = createFile->createElement(_bstr_t(rootTag), &createRoot);
// 
// 	assert(hr);
// 
// 	hr = createFile->appendChild(createRoot, NULL);
// 
// 	assert(hr);
// 
// 	return createFile;
// 
// }
// 
// IXMLDOMDocumentPtr XMLProc::Read(LPCTSTR sfile)
// 
// {
// 
// 	VARIANT_BOOL dl;
// 
// 	hr = readFile.CreateInstance(__uuidof(DOMDocument));
// 
// 	assert(hr);
// 
// 	hr = readFile->load(_variant_t(sfile), &dl);
// 
// 	assert(hr);
// 
// 	hr = readFile->get_documentElement(&readRoot);
// 
// 	assert(hr);
// 
// 	return readFile;
// 
// }
// 
// BOOL XMLProc::NewElement(LPCTSTR tag, LPCTSTR text, IXMLDOMElement** newNode)
// 
// {
// 
// 	if (createFile == NULL || tag == NULL || text == NULL){
// 
// 		return FALSE;
// 
// 	}
// 
// 	hr = createFile->createElement(_bstr_t(tag), newNode);
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	(*newNode)->put_text(_bstr_t(text));
// 
// 
// 
// 	return TRUE;
// 
// }
// 
// 
// 
// BOOL XMLProc::NewElement(LPCTSTR tag, IXMLDOMElement** newNode)
// 
// {
// 
// 	if (createFile == NULL || tag == NULL){
// 
// 		return FALSE;
// 
// 	}
// 
// 	hr = createFile->createElement(_bstr_t(tag), newNode);
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 
// 
// 	return TRUE;
// 
// }
// 
// 
// 
// BOOL XMLProc::SelectElement(LPCTSTR tag, IXMLDOMElement** node)
// 
// {
// 
// 	TCHAR usetag[MAX_STRING + 2] = TEXT("//");
// 
// 	if (readFile == NULL || tag == NULL || _tcslen(tag) >= MAX_STRING)
// 
// 		return FALSE;
// 
// 	_tcscat_s(usetag, MAX_STRING, tag);
// 
// 	hr = readFile->selectSingleNode(_bstr_t(usetag), (IXMLDOMNode**)node);
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	return TRUE;
// 
// }
// 
// 
// 
// BOOL XMLProc::GetText(IXMLDOMElement *elem, LPTSTR text, int nBufferSize, int* tlen)
// 
// {
// 
// 	BSTR bst;
// 
// 	_bstr_t bt;
// 
// 	if (elem == NULL || text == NULL)
// 
// 		return FALSE;
// 
// 	hr = elem->get_text(&bst);
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	else {
// 
// 		bt = bst;
// 
// 		*tlen = (int)_tcslen((LPCTSTR)bt) > *tlen ? *tlen : (int)_tcslen((LPCTSTR)bt);
// 
// 		_tcsncpy_s(text, nBufferSize, (LPCTSTR)bt, *tlen);
// 
// 		text[*tlen] = 0;
// 
// 		return TRUE;
// 
// 	}
// 
// }
// 
// 
// 
// BOOL XMLProc::ChangeText(IXMLDOMElement *elem, LPCTSTR text)
// 
// {
// 
// 	if (elem == NULL)
// 
// 		return FALSE;
// 
// 	hr = elem->put_text(_bstr_t(text));
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	else {
// 
// 		return TRUE;
// 
// 	}
// 
// }
// 
// 
// 
// BOOL XMLProc::AddChild(IXMLDOMElement* childElem, IXMLDOMElement* parentElem)
// 
// {
// 
// 	if (parentElem == NULL || childElem == NULL)
// 
// 		return FALSE;
// 
// 	hr = parentElem->appendChild((IXMLDOMNode*)childElem, NULL);
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	else
// 
// 		return TRUE;
// 
// }
// 
// 
// 
// BOOL XMLProc::GetParent(IXMLDOMElement* childElem, IXMLDOMElement** parentElem)
// 
// {
// 
// 	if (childElem == NULL)
// 
// 		return FALSE;
// 
// 	hr = childElem->get_parentNode((IXMLDOMNode**)parentElem);
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	else
// 
// 		return TRUE;
// 
// }
// 
// 
// 
// BOOL XMLProc::GetChild(IXMLDOMElement** childElem, IXMLDOMElement* parentElem)
// 
// {
// 
// 	if (parentElem == NULL)
// 
// 		return FALSE;
// 
// 	hr = parentElem->get_firstChild((IXMLDOMNode**)childElem);
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	else
// 
// 		return TRUE;
// 
// }
// 
// 
// 
// BOOL XMLProc::SetAttribute(IXMLDOMElement* node, LPCTSTR attr, LPCTSTR value)
// 
// {
// 
// 	if (node == NULL){
// 
// 		return FALSE;
// 
// 	}
// 
// 	hr = node->setAttribute(_bstr_t(attr), _variant_t(value));
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	else
// 
// 		return TRUE;
// 
// }
// 
// 
// 
// BOOL XMLProc::RemoveChild(IXMLDOMElement* childElem, IXMLDOMElement* parentElem)
// 
// {
// 
// 	if (parentElem == NULL || childElem == NULL)
// 
// 		return FALSE;
// 
// 	hr = parentElem->removeChild((IXMLDOMNode*)childElem, NULL);
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	else
// 
// 		return TRUE;
// 
// }
// 
// 
// 
// BOOL XMLProc::SaveCreateFile(LPCTSTR dfile)
// 
// {
// 
// 	if (createFile == NULL || dfile == NULL)
// 
// 		return FALSE;
// 
// 	hr = createFile->save(_variant_t(dfile));
// 
// 	if (hr != true){
// 
// 		return FALSE;
// 
// 	}
// 
// 	else
// 
// 		return TRUE;
// 
// }
// 
// 
// 
// BOOL XMLProc::SaveReadFile(LPCTSTR dfile)
// 
// {
// 
// 	if (readFile == NULL || dfile == NULL)
// 
// 		return FALSE;
// 
// 	hr = readFile->save(_variant_t(dfile));
// 
// 	if (hr != true)
// 
// 		return FALSE;
// 
// 	else
// 
// 		return TRUE;
// 
// }
// 
// 
// 
// IXMLDOMDocumentPtr XMLProc::GetCreateFile()
// 
// {
// 
// 	return createFile;
// 
// }
// 
// 
// 
// IXMLDOMDocumentPtr XMLProc::GetReadFile()
// 
// {
// 
// 	return readFile;
// 
// }
// 
// 
// 
// IXMLDOMElement* XMLProc::GetCreateRoot()
// 
// {
// 
// 	return createRoot;
// 
// }
// 
// 
// 
// IXMLDOMElement* XMLProc::GetReadRoot()
// 
// {
// 
// 	return readRoot;
// 
// }
// 
// 
// 
// void XMLProc::Fail()
// 
// {
// 
// 	if (readRoot)
// 
// 		readRoot.Release();
// 
// 	if (readFile)
// 
// 		readFile.Release();
// 
// 	if (createRoot)
// 
// 		createRoot.Release();
// 
// 	if (pPI)
// 
// 		pPI.Release();
// 
// 	if (createFile)
// 
// 		createFile.Release();
// 
// 
// 
// }
// 
// 
// 
// void XMLProc::Exit()
// 
// {
// 
// 	Fail();
// 
// 	//CoUninitialize();
// 
// }