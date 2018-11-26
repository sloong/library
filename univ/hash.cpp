#include "stdafx.h"
#include <assert.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include "exception.h"
#include "hash.h"

using namespace Sloong::Universal;

void CMD5::Binary_Encode(string str, unsigned char(&md)[MD5_LENGTH], bool bFile)
{
	if (bFile)
	{
		FILE *fd = fopen(str.c_str(), "rb");
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



string CMD5::Encode(string str, bool bFile /*= false*/)
{
	unsigned char md5[MD5_LENGTH] = { 0 };

	Binary_Encode(str, md5, bFile);
	return CUniversal::BinaryToHex(md5, MD5_LENGTH);
}



string Sloong::Universal::CSHA1::Encode(string str_src, bool file )
{
	unsigned char mdStr[SHA1_LENGTH] = { 0 };
	Binary_Encoding(str_src, mdStr);
	return CUniversal::BinaryToHex(mdStr, SHA1_LENGTH);
}

void Sloong::Universal::CSHA1::Binary_Encoding(const string& str_src, unsigned char(&md)[SHA1_LENGTH], bool bFile )
{
	if (bFile)
	{
		SHA_CTX sha1;

		SHA1_Init(&sha1);

		FILE *fd = fopen(str_src.c_str(), "r");
		int len;
		unsigned char buffer[1024] = { '\0' };
		while (0 != (len = fread(buffer, 1, 1024, fd)))
		{
			SHA1_Update(&sha1, buffer, len);
		}
		fclose(fd);
		SHA1_Final(md, &sha1);
	}
	else
	{
		SHA1((const unsigned char *)str_src.c_str(), str_src.length(), md);
	}
}


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
