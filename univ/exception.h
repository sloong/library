#pragma once
#include "univ.h"
#include "string/string.h"
#pragma comment(lib,"string.lib")
namespace Sloong
{
	namespace Universal
	{
		class UNIVERSAL_API CException
		{
		public:
			CException(CString lpstr);
			CException(CString lpStr, HRESULT hRes);
			~CException();

		public:
			CString GetException() const;
			HRESULT GetResult() const;

		protected:
			HRESULT m_hResult;
			CString m_strMessage;
		};
	}
}