#pragma once
#include "univ.h"

namespace Sloong
{
	namespace Universal
	{
		const int SHA256_DIGEST_LENGTH = 32;
		const int SHA512_DIGEST_LENGTH = 64;
		class UNIVERSAL_API CSHA256
		{
		public:
			CSHA256() {}
			~CSHA256() {}

			// compare string or file hash value.
			static string Encode(string str_src, bool file = false);
			static void Binary_Encoding( const string& str_src, unsigned char (&md)[Sloong::Universal::SHA256_DIGEST_LENGTH], bool bFile = false);
		};

		class UNIVERSAL_API CSHA512
		{
		public:
			CSHA512() {}
			~CSHA512() {}

			// compare string or file hash value.
			static string Encode(string str_src, bool file = false);
			static void Binary_Encoding(const string& str_src, unsigned char(&md)[Sloong::Universal::SHA512_DIGEST_LENGTH], bool bFile = false);
		};
	}
}

