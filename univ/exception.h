#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include "univ.h"
#include "string/string.h"
#pragma comment(lib,"string.lib")
namespace Sloong
{
	namespace Universal
	{
		class UNIVERSAL_API normal_except : exception
		{
		public:
			normal_except();
			normal_except(CString lpstr);
			normal_except(const normal_except&);
			// in windows os ,the hRes is GetLastError function, in linux os the hRes is errno
			normal_except(CString lpStr, long hRes);
			normal_except& operator= (const normal_except&);
			virtual ~normal_except();
			virtual const char* what() const noexcept;
			
		protected:
			long m_hResult;
			CString m_strMessage;
		};
	}
}

#endif // !EXCEPTION_H
