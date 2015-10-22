#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "univ.h"
#ifdef _WINDOWS
#endif
namespace Sloong
{
	namespace Universal
	{
		class UNIVERSAL_API normal_except
		{
		public:
			normal_except();
			normal_except(std::string lpstr);
			normal_except(const normal_except&);
			// in windows os ,the hRes is GetLastError function, in linux os the hRes is errno
			normal_except(std::string lpStr, long hRes);
			normal_except& operator= (const normal_except&);
			virtual ~normal_except();
			virtual const char* what() const;
			
		protected:
			long m_hResult;
			std::string m_strMessage;
		};
	}
}

#endif // !EXCEPTION_H
