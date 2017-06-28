#include "stdafx.h"
#include "DES.h"
#include <openssl/des.h>
using namespace Sloong::Universal;


int CDES::Get_Length(const char* datain)
{
	int data_len = strlen(datain);
	int data_rest = data_len % 8;
	return data_len + (8 - data_rest);
}

int CDES::Encrypt(const char* datain, unsigned char* dataout, const unsigned char* keyin)
{
	int docontinue = 1;

	int data_len;
	int data_rest;
	unsigned char ch;

	unsigned char *src = NULL;
	unsigned char *dst = NULL;
	int len;
	unsigned char tmp[8];
	unsigned char in[8];
	unsigned char out[8];

	int key_len;
#define LEN_OF_KEY              24
	unsigned char key[LEN_OF_KEY]; /* 补齐后的密钥 */
	unsigned char block_key[9];
	DES_key_schedule ks, ks2, ks3;

	key_len = strlen((const char*)keyin);
	memcpy(key, keyin, key_len);
	memset(key + key_len, '0', LEN_OF_KEY - key_len);

	data_len = strlen(datain);
	data_rest = data_len % 8;
	len = data_len + (8 - data_rest);
	//len = data_len;
	ch = 8 - data_rest;

	src = (unsigned char *)malloc(len);
	dst = (unsigned char *)malloc(len);
	if (NULL == src || NULL == dst)
	{
		docontinue = 0;
	}
	if (docontinue)
	{
		int count;
		int i;

		/* 构造补齐后的加密内容 */
		memset(src, 0, len);
		memcpy(src, datain, data_len);
		memset(src + data_len, ch, 8 - data_rest);
		//memset(src + data_len, ch, '8');
		//memset(src + data_len, ch, '0');


		memset(block_key, 0, sizeof(block_key));
		memcpy(block_key, key + 0, 8);
		DES_set_key_unchecked((const_DES_cblock*)block_key, &ks);
		memcpy(block_key, key + 8, 8);
		DES_set_key_unchecked((const_DES_cblock*)block_key, &ks2);
		memcpy(block_key, key + 16, 8);
		DES_set_key_unchecked((const_DES_cblock*)block_key, &ks3);

		count = len / 8;
		for (i = 0; i < count; i++)
		{
			memset(tmp, 0, 8);
			memset(in, 0, 8);
			memset(out, 0, 8);
			memcpy(tmp, src + 8 * i, 8);
			//# define DES_ecb2_encrypt(i,o,k1,k2,e)
			//DES_ecb3_encrypt((i),(o),(k1),(k2),(k1),(e))
			DES_ecb3_encrypt((const_DES_cblock*)tmp, (DES_cblock*)(dst + 8 * i), &ks, &ks2, &ks3, DES_ENCRYPT);

		}
	}
	std::string str;
	str.assign(dst, dst + len);
	int j = 0;
	for (j = 0; j < len; j++)
	{
		//本来可以省掉，
		dataout[j] = dst[j];
		//_snprintf(dataout+ j*2, 3, "%c", dst[j]);
		//_snprintf(dataout+ j*2, 3, "%02x", dst[j]);
		//TRACE("%d-%c%c\r\n",j,dataout[j],dataout[j+1]);
		//_snprintf(dataout+ j*2, 3, "%c", dst[j]);
		//_snprintf(dataout+ j, 1, "%c", dst[j]);
	}

	if (NULL != src)
	{
		free(src);
		src = NULL;
	}
	if (NULL != dst)
	{
		free(dst);
		dst = NULL;
	}

	return len;
}
