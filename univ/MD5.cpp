#include "stdafx.h"
#include "MD5.h"
#include <assert.h>
#ifdef _WINDOWS
#include <Wincrypt.h>
#include <direct.h> 
#else
#include <openssl/md5.h>
#endif
#include "exception.h"

using namespace Sloong::Universal;

void CMD5::Binary_Encoding(string str, unsigned char* md, bool bFile)
{
#ifdef _WINDOWS
	// use the windows api
	HINSTANCE hDLL = LoadLibrary(L"Cryptdll.dll");
	if (hDLL == NULL)
	{
		throw normal_except("load cryptdll error.");
	}
	MD5Init_Tpye   MD5Init;
	MD5Update_Tpye MD5Update;
	MD5Final_Tpye  MD5Final;
	MD5Init = (MD5Init_Tpye)GetProcAddress(hDLL, "MD5Init");
	MD5Update = (MD5Update_Tpye)GetProcAddress(hDLL, "MD5Update");
	MD5Final = (MD5Final_Tpye)GetProcAddress(hDLL, "MD5Final");
	if (MD5Init == NULL || MD5Update == NULL || MD5Final == NULL)
	{
		FreeLibrary(hDLL);
		throw normal_except("get md5 function from cryptdll error.");
	}
	MD5_CTX md5_context;
	MD5Init(&md5_context);

	if (bFile)
	{
		FILE *fd = fopen(str.c_str(), "r");
		int len;
		unsigned char buffer[1024] = { '\0' };
		while (0 != (len = fread(buffer, 1, 1024, fd)))
		{
			MD5Update(&md5_context, buffer, len);
		}
		fclose(fd);
	}
	else
	{
		unsigned char* src = new unsigned char[str.size() + 1];
		memset(src, 0, str.size() + 1);
		memcpy(src, str.c_str(), str.size());
		MD5Update(&md5_context, src, str.size());
		SAFE_DELETE_ARR(src)
	}
	MD5Final(&md5_context);

	for (int i = 0; i < 16; ++i)
	{
		md[i] = md5_context.digest[i];
	}
	FreeLibrary(hDLL);
#else
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
#endif // _WINDOWS
}



string CMD5::Encoding(string str, bool bFile /*= false*/)
{
	unsigned char md5[16] = { 0 };

	Binary_Encoding(str, md5, bFile);
	char dest[35] = { 0 };
	char *p = dest;
	for (int i = 0; i < 16; ++i)
	{
		sprintf(p,  "%02x", md5[i]);
		p += 2;
	}

	return dest;
}
