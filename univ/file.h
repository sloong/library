#pragma once
#include "univ.h"
#include "string/string.h"
namespace Sloong
{
	namespace Universal
	{
		typedef enum OpenFileAccess
		{
			Exist = 0,
			Write = 2,
			Read = 4,
			AllAccess = Exist | Write | Read,
		}FILEACCESS;
		class UNIVERSAL_API CFile
		{
		public:
			CFile();
			virtual ~CFile();

			virtual LPCTSTR GetLine();
			virtual HRESULT Open(CString szFileName, OpenFileAccess emMode);
			virtual errno_t OpenStream(CString szFileName, CString szMode);
			virtual void Close();
		public:
			static bool Access(CString szFileName, OpenFileAccess emMode);
		public:
			TCHAR m_szBuffer[256];
			FILE* m_pFileStream;
			CString m_strFileName;
		};
	}
}

