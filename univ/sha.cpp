#include "stdafx.h"
#include "sha.h"
#include "openssl/sha.h"

using namespace Sloong::Universal;

string Sloong::Universal::CSHA256::Encode(string str_src, bool file )
{
	unsigned char mdStr[SHA256_LENGTH] = { 0 };
	Binary_Encoding(str_src, mdStr);
	return CUniversal::BinaryToHex(mdStr, SHA256_LENGTH);
}

void Sloong::Universal::CSHA256::Binary_Encoding(const string& str_src, unsigned char(&md)[SHA256_LENGTH], bool bFile )
{
	if (bFile)
	{
		SHA256_CTX sha256;

		SHA256_Init(&sha256);

		FILE *fd = fopen(str_src.c_str(), "r");
		int len;
		unsigned char buffer[1024] = { '\0' };
		while (0 != (len = fread(buffer, 1, 1024, fd)))
		{
			SHA256_Update(&sha256, buffer, len);
		}
		fclose(fd);
		SHA256_Final(md, &sha256);
	}
	else
	{
		SHA256((const unsigned char *)str_src.c_str(), str_src.length(), md);
	}
}

string Sloong::Universal::CSHA512::Encode(string str_src, bool file)
{
	unsigned char mdStr[SHA512_LENGTH] = { 0 };
	Binary_Encoding(str_src, mdStr);
	return CUniversal::BinaryToHex(mdStr, SHA512_LENGTH);
}

void Sloong::Universal::CSHA512::Binary_Encoding(const string & str_src, unsigned char(&md)[SHA512_LENGTH], bool bFile)
{
	if (bFile)
	{
		SHA512_CTX sha512;

		SHA512_Init(&sha512);

		FILE *fd = fopen(str_src.c_str(), "r");
		int len;
		unsigned char buffer[1024] = { '\0' };
		while (0 != (len = fread(buffer, 1, 1024, fd)))
		{
			SHA512_Update(&sha512, buffer, len);
		}
		fclose(fd);
		SHA512_Final(md, &sha512);
	}
	else
	{
		SHA512((const unsigned char *)str_src.c_str(), str_src.length(), md);
	}
}
