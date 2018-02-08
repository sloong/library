#include "stdafx.h"
#include "Base64.h"
#include <math.h>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

using namespace Sloong::Universal;

// Encoding lookup table
char base64encode_lut[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
	'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
	'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '=' };


// Decode lookup table
char base64decode_lut[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 62, 0, 0, 0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
	49, 50, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

bool Base64Encode(const string& input, string* output) {
	typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<string::const_iterator, 6, 8> > Base64EncodeIterator;
	stringstream result;
	copy(Base64EncodeIterator(input.begin()), Base64EncodeIterator(input.end()), ostream_iterator<char>(result));
	size_t equal_count = (3 - input.length() % 3) % 3;
	for (size_t i = 0; i < equal_count; i++) {
		result.put('=');
	}
	*output = result.str();
	return output->empty() == false;
}

bool Base64Decode(const string& input, string* output) {
	typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<string::const_iterator>, 8, 6> Base64DecodeIterator;
	stringstream result;
	try {
		copy(Base64DecodeIterator(input.begin()), Base64DecodeIterator(input.end()), ostream_iterator<char>(result));
	}
	catch (...) {
		return false;
	}
	*output = result.str();
	return output->empty() == false;
}


CBase64::CBase64()
{
}


CBase64::~CBase64()
{
}

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}


string CBase64::Encoding(const unsigned char *str, int length)
{
	/* {{{ base64 tables */
	static const char base64_pad = '=';

	const unsigned char *current = str;
	unsigned char *p;
	unsigned char * result;

	result = new unsigned char[(length + 2) / 3 * 4 + 1];
	p = result;

	if (length < 0) {
		return NULL;
	}

	while (length > 2) { /* keep going until we have less than 24 bits */
		*p++ = base64encode_lut[current[0] >> 2];
		*p++ = base64encode_lut[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		*p++ = base64encode_lut[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		*p++ = base64encode_lut[current[2] & 0x3f];

		current += 3;
		length -= 3; /* we just handle 3 octets of data */
	}

	/* now deal with the tail end of things */
	if (length != 0) {
		*p++ = base64encode_lut[current[0] >> 2];
		if (length > 1) {
			*p++ = base64encode_lut[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			*p++ = base64encode_lut[(current[1] & 0x0f) << 2];
			*p++ = base64_pad;
		}
		else {
			*p++ = base64encode_lut[(current[0] & 0x03) << 4];
			*p++ = base64_pad;
			*p++ = base64_pad;
		}
	}
	*p = '\0';
	string res = (char*)result;
	SAFE_DELETE_ARR(result);
	return res;
}


string CBase64::deciToBin_6bit(int deci)                                //10进制转2进制函数
{

	string hexStr(6, ' ');
	int Value = 0;
	int i = 0;

	for (; deci != 0; ++i, deci /= 2)
	{
		Value = deci % 2;
		hexStr.at(i) = Value + '0';
	}

	hexStr = hexStr.substr(0, i);

	reverse(hexStr.begin(), hexStr.end());

	if (hexStr.length() < 6)
	{
		string add(6 - hexStr.length(), '0');
		hexStr = add + hexStr;
	}

	return hexStr;
}
string CBase64::Decoding(string str)
{
	string value("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	string out0;
	string out1;

	for (auto x = str.cbegin(); (*x) != '=' && x != str.cend(); ++x)    //排除等号
		out0 += deciToBin_6bit(value.find(*x));

	const int n = out0.length();

	for (int i = 0; i < n; i += 8)           //每8位提取一次
	{
		string subs = out0.substr(i, 8);    //提取8位
		int j = 0;
		int sum = 0;

		for (auto &x : subs)
		{
			sum += (x - '0') * pow(2, 7 - j);
			++j;
		}                                   //按权展开，得到ASCII码

		out1 += static_cast<char>(sum);     //转换为相应字符
	}

	return out1;
}

int CBase64::Binary_Decoding(string strBase64, unsigned char * outBuf)
{
	const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int i, j;
	unsigned char k;
	unsigned char temp[4];
	const char* base64 = strBase64.c_str();
	for (i = 0, j = 0; base64[i] != '\0'; i += 4)
	{
		memset(temp, 0xFF, sizeof(temp));
		for (k = 0; k < 64; k++)
		{
			if (base64char[k] == base64[i])
				temp[0] = k;
		}
		for (k = 0; k < 64; k++)
		{
			if (base64char[k] == base64[i + 1])
				temp[1] = k;
		}
		for (k = 0; k < 64; k++)
		{
			if (base64char[k] == base64[i + 2])
				temp[2] = k;
		}
		for (k = 0; k < 64; k++)
		{
			if (base64char[k] == base64[i + 3])
				temp[3] = k;
		}

		outBuf[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2)) & 0xFC)) |
			((unsigned char)((unsigned char)(temp[1] >> 4) & 0x03));
		if (base64[i + 2] == '=')
			break;

		outBuf[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4)) & 0xF0)) |
			((unsigned char)((unsigned char)(temp[2] >> 2) & 0x0F));
		if (base64[i + 3] == '=')
			break;

		outBuf[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6)) & 0xF0)) |
			((unsigned char)(temp[3] & 0x3F));
	}
	return j;
}
