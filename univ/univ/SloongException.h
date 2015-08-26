#pragma once
#include "SloongUniversal.h"
#include <exception>
namespace Sloong
{
	namespace Universal
	{
		using std::exception;
		class UNIVERSAL_API CException : public exception
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