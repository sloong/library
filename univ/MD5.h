#pragma once
#include "univ.h"

namespace Sloong
{
	namespace Universal
	{
		class UNIVERSAL_API CMD5
		{
		public:
			CMD5() {}
			~CMD5() {}

			static void Binary_Encoding(string str, unsigned char* md, bool bFile = false);
			static string Encoding(string str, bool bFile = false);
		};


#ifdef _WINDOWS
		typedef struct
		{
			ULONG i[2];
			ULONG buf[4];
			unsigned char in[64];
			unsigned char digest[16];
		} MD5_CTX;
		typedef void (CALLBACK* MD5Init_Tpye)(MD5_CTX* context);
		typedef void (CALLBACK* MD5Update_Tpye)(MD5_CTX* context, unsigned char* input, unsigned int inlen);
		typedef void (CALLBACK* MD5Final_Tpye)(MD5_CTX* context);
#endif // _WINDOWS

	}
}
