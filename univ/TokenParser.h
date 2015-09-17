#ifndef TOKENPARSER_H
#define TOKENPARSER_H


class CTokenParser
{
public:
	CTokenParser(void);
	~CTokenParser(void);
	LRESULT Initialize(LPCTSTR strFilePath);
	void Shutdown();
	//--- Reset Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		It just used in TokenStream Mode.
	void Reset();

	//--- SetTokenStream Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		It just used in TokenStream mode
	LRESULT SetTokenStream(LPTSTR strData);

	//--- GetNextToken Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		It just used in TokenStream Mode.
	LRESULT GetNextToken(LPTSTR buffer, UINT BufSize);
	LRESULT GetNextToken(LPTSTR token, LPTSTR buffer, UINT nBufSize);

	//--- IsValidIdentifier Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		Just used in TokenStream Mode.
	bool IsValidIdentifier(TCHAR c);

	//--- MoveToNextLine Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		Just used in TokenStream
	bool MoveToNextLine(LPTSTR buffer, int BufSize);
	//--- 0000014 --- 2013/10/24 --- WCB --- Add
	// Just used in TokenStream Mode.
	//--- 0000017 --- 2013/10/25 --- WCB --- Modify

protected:
	UINT m_unTotalTokens;
	UINT m_unStartIndex, m_unEndIndex;
	UINT m_unLength;

	LPTSTR m_strData;
};

class CTokenParser
{
public:
	CTokenParser(void);
	~CTokenParser(void);
	HRESULT Initialize(LPCTSTR strFilePath);
	void Shutdown();
	//--- Reset Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		It just used in TokenStream Mode.
	void Reset();

	//--- SetTokenStream Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		It just used in TokenStream mode
	HRESULT SetTokenStream(LPTSTR strData);

	//--- GetNextToken Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		It just used in TokenStream Mode.
	HRESULT GetNextToken(LPTSTR buffer, UINT BufSize);
	HRESULT GetNextToken(LPTSTR token, LPTSTR buffer, UINT nBufSize);

	//--- IsValidIdentifier Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		Just used in TokenStream Mode.
	bool IsValidIdentifier(TCHAR c);

	//--- MoveToNextLine Function annotation ---
	// Parameters:
	//		
	// Return value:
	//		
	// Remarks:
	//		Just used in TokenStream
	bool MoveToNextLine(LPTSTR buffer, int BufSize);
	//--- 0000014 --- 2013/10/24 --- WCB --- Add
	// Just used in TokenStream Mode.
	//--- 0000017 --- 2013/10/25 --- WCB --- Modify

	int GetTotalLine();
protected:
	UINT m_unTotalTokens;
	UINT m_unStartIndex, m_unEndIndex;
	UINT m_unLength;

	LPTSTR m_strData;
};

#endif // !TOKENPARSER_H
