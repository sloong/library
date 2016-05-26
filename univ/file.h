#ifndef FILE_H
#define FILE_H


#include "univ.h"
#include "string.h"
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
			virtual HRESULT Open(const wstring& szFileName, OpenFileAccess emMode);
			virtual errno_t OpenStream(const wstring& szFileName, const wstring& szMode);
			virtual void Close();
		public:
			static bool Access(const wstring& szFileName, OpenFileAccess emMode);
		public:
			TCHAR m_szBuffer[256];
			FILE* m_pFileStream;
			wstring m_strFileName;
		};
	}
}

#endif // !FILE_H
