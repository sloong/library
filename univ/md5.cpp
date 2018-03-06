#include "stdafx.h"
#include "md5.h"
#include <assert.h>
#include <openssl/md5.h>
#include "exception.h"

using namespace Sloong::Universal;

void CMD5::Binary_Encoding(string str, unsigned char(&md)[MD5_DIGEST_LENGTH], bool bFile)
{
	if (bFile)
	{
		FILE *fd = fopen(str.c_str(), "r");
		MD5_CTX c;
		int len;
		unsigned char buffer[1024] = { '\0' };
		MD5_Init(&c);
		while (0 != (len = fread(buffer, 1, 1024, fd)))
		{
			MD5_Update(&c, buffer, len);
		}
		MD5_Final(md, &c);
		fclose(fd);
	}
	else
	{
		MD5((unsigned char *)str.c_str(), str.length(), md);
	}
}



string CMD5::Encoding(string str, bool bFile /*= false*/)
{
	unsigned char md5[MD5_DIGEST_LENGTH] = { 0 };

	Binary_Encoding(str, md5, bFile);
	return CUniversal::BinaryToHex(md5, MD5_DIGEST_LENGTH);
}
