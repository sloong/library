#pragma once
#include "univ.h"

namespace Sloong
{
	namespace Universal
	{
		class UNIVERSAL_API CBase64
		{
		public:
			CBase64();
			~CBase64();

			static string Encoding(const unsigned char* str, int len);
			static string Decoding(string str);
			static string deciToBin_6bit(int deci);
			static int Binary_Decoding(string strBase64, unsigned char * outBuf);
		};


	}
}
