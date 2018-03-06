#pragma once
#include "univ.h"

namespace Sloong
{
	namespace Universal
	{
		const int MD5_DIGEST_LENGTH = 16;
		class UNIVERSAL_API CMD5
		{
		public:
			CMD5() {}
			~CMD5() {}

			static void Binary_Encoding(string str, unsigned char(&md)[Sloong::Universal::MD5_DIGEST_LENGTH], bool bFile = false);
			static string Encoding(string str, bool bFile = false);
		};
	}
}