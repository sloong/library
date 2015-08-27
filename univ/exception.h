#pragma once
#include "univ.h"
namespace Sloong
{
	namespace Universal
	{
		class UNIVERSAL_API CException
		{
		public:
			CException(wstring lpstr);
			CException(wstring lpStr, HRESULT hRes);
			~CException();

		public:
			wstring GetException() const;
			HRESULT GetResult() const;

		protected:
			HRESULT m_hResult;
			wstring m_strMessage;
		};
	}
}