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
            normal_except(){}
            normal_except(std::string lpstr){m_strMessage = lpstr;}
			// in windows os ,the hRes is GetLastError function, in linux os the hRes is errno
            normal_except(std::string lpStr, long hRes)
            {
                m_strMessage = lpStr;
                m_hResult = hRes;
            }
            normal_except& operator= (const normal_except&){return (*this);}
            virtual ~normal_except(){}
            virtual const char* what() const{return m_strMessage.c_str();}
			
		protected:
			long m_hResult;
			std::string m_strMessage;
		};

        class UNIVERSAL_API CExceptKeyNoFound : public normal_except
        {
        public:
            CExceptKeyNoFound(){}
            CExceptKeyNoFound(std::string lpstr){m_strMessage = lpstr;}
            // in windows os ,the hRes is GetLastError function, in linux os the hRes is errno
            CExceptKeyNoFound(std::string lpStr, long hRes)
            {
                m_strMessage = lpStr;
                m_hResult = hRes;
            }
            CExceptKeyNoFound& operator= (const CExceptKeyNoFound&){return (*this);}
            virtual ~CExceptKeyNoFound(){}
        };
	}
}

#endif // !EXCEPTION_H
