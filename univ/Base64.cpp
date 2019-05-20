#include "stdafx.h"
#include "Base64.h"
#include <math.h>

using namespace Sloong::Universal;

// Encoding lookup table
char base64encode_lut[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
	'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
	'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

const char padding_char = '=';


// Decode lookup table
char base64decode_lut[] = {	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	62, // '+'
	0, 0, 0, 
	63, // '/'
	52, 53, 54, 55,	56, 57, 58, 59, 60, 61,  // '0'-'9'
	0,  0,  0,  0,  0,  0,  0,  
	0,  1,  2,  3,  4,  5,  6,	7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
	0,	0,  0,  0, 0,  0, 
	26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51  // 'a'-'z'
	};



string CBase64::Encode(const string& strEncode)
{
	auto src = (const unsigned char*)strEncode.data();
	int len = strEncode.length();
	unsigned char *out, *pos;
    const unsigned char *end, *in;

    size_t olen;

    olen = 4*((len + 2) / 3); /* 3-byte blocks to 4-byte */

    if (olen < len)
        return std::string(); /* integer overflow */

    std::string outStr;
    outStr.resize(olen);
    out = (unsigned char*)&outStr[0];

	end = src + len;
	in = src;
	pos = out;
	while (end - in >= 3) {
		*pos++ = base64encode_lut[in[0] >> 2];
		*pos++ = base64encode_lut[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64encode_lut[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64encode_lut[in[2] & 0x3f];
		in += 3;
	}

	if (end - in) {
		*pos++ = base64encode_lut[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = base64encode_lut[(in[0] & 0x03) << 4];
			*pos++ = padding_char;
		}
		else {
			*pos++ = base64encode_lut[((in[0] & 0x03) << 4) |
				(in[1] >> 4)];
			*pos++ = base64encode_lut[(in[1] & 0x0f) << 2];
		}
		*pos++ = padding_char;
	}

    return outStr;
}


string CBase64::Decode(const string& strDecode)
{
	unsigned char* p = (unsigned char*)strDecode.data();
	int len = strDecode.length();
    int pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const size_t L = ((len + 3) / 4 - pad) * 4;
    std::string str(L / 4 * 3 + pad, '\0');

    for (size_t i = 0, j = 0; i < L; i += 4)
    {
        int n = base64decode_lut[p[i]] << 18 | base64decode_lut[p[i + 1]] << 12 | base64decode_lut[p[i + 2]] << 6 | base64decode_lut[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad)
    {
        int n = base64decode_lut[p[L]] << 18 | base64decode_lut[p[L + 1]] << 12;
        str[str.size() - 1] = n >> 16;

        if (len > L + 2 && p[L + 2] != '=')
        {
            n |= base64decode_lut[p[L + 2]] << 6;
            str.push_back(n >> 8 & 0xFF);
        }
    }
    return str;
}
