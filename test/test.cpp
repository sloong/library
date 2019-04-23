// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Base64.h"
#include "BlowFish.h"
#include "hash.h"
#include "log.h"
#include "luapacket.h"
#pragma comment(lib,"univ.lib")
using namespace Sloong::Universal;
typedef string(*Func)(string orgTxt, string key, BOOL bBase64);
typedef struct FuncListItem
{
	LPCTSTR FuncName;
	Func pFunc;
}FuncListItem;


string Base64Encoding(string orgTxt, string key, BOOL bBase64);
string MD5Encoding(string orgTxt, string key, BOOL bBase64);
string Base64Decoding(string orgTxt, string key, BOOL bBase64);
string BinaryMD5Base64Encoding(string orgTxt, string key, BOOL bBase64);
string BlowFishAyersEncoding(string orgTxt, string key, BOOL bBase64);
string BlowFishAyersDecoding(string orgTxt, string key, BOOL bBase64);
string BlowFish2AyersEncoding(string orgTxt, string key, BOOL bBase64);
string BlowFishEncoding(string orgTxt, string key, BOOL bBase64);
string Base64Ayers2Blowfish(string orgTxt, string key, BOOL bBase64);
string LogSystemTest(string orgTxt, string key, BOOL bBase64);


FuncListItem FuncList[] =
{
	{ "Base64 Encoding", Base64Encoding },
	{ "Base64 Decoding", Base64Decoding },
	{ "BlowFish Encoding", BlowFishEncoding },
	{ "二进制MD5+Base64", BinaryMD5Base64Encoding },
	{ "MD5 Encoding", MD5Encoding},
	{ "BlowFish Ayers Encoding", BlowFishAyersEncoding },
	{ "BlowFish Ayers Decoding", BlowFishAyersDecoding },
	{ "BlowFish->Ayers Encoding", BlowFish2AyersEncoding },
	{ "Base64Ayers->BlowFish", Base64Ayers2Blowfish },
	{ "Base64Ayers->BlowFish", Base64Ayers2Blowfish },
	{"logSystem",LogSystemTest},
};



string Base64Encoding(string orgTxt, string key, BOOL bBase64)
{
	return CBase64::Encode(orgTxt);
}


std::string MD5Encoding(string orgTxt, string key, BOOL bBase64)
{
	auto res = CMD5::Encode(orgTxt, FALSE);
	if (bBase64)
	{
		return CBase64::Encode(res);
	}
	return res;
}

string Base64Decoding(string orgTxt, string key, BOOL bBase64)
{
	return CBase64::Decode(orgTxt);
}

string BinaryMD5Base64Encoding(string orgTxt, string key, BOOL bBase64)
{
	unsigned char md[16] = { 0 };
	CMD5::Binary_Encode(orgTxt, md);

	return CBase64::Encode(string((char*)md, 16));
}


string BlowFishAyersEncoding(string orgTxt, string key, BOOL bBase64)
{
	CBlowFish blowfish((unsigned char*)key.c_str(), key.length());
	string strEncodeData;
	strEncodeData.resize(3 * orgTxt.length());
	blowfish.Encrypt((unsigned char*)orgTxt.c_str(), (unsigned char*)strEncodeData.data(), orgTxt.length());
	string strAyersData("");
	strAyersData.resize(strEncodeData.length());
	int nAyersData = CBlowFish::CompressBlowfish(strEncodeData.data(), (char*)strAyersData.data());

	return CBase64::Encode(strAyersData);
}


string BlowFishAyersDecoding(string orgTxt, string key, BOOL bBase64)
{
	/*CString strBase64;
	string res = CBase64::Decode(orgTxt);
	const char* pBuf = res.c_str();
	int nLenMsg = 0;
	memcpy(&nLenMsg, (LPCTSTR)pBuf, 4);
	CString strChgBuffer("");
	int nEncodeDataLength = CBlowFish::UncompressBlowfish(pBuf + 4, strChgBuffer.GetBuffer(nLenMsg), nLenMsg);
	strChgBuffer.ReleaseBuffer();
	CBlowFish blowfish((unsigned char*)key.c_str(), key.length());
	CString strDecDataBuffer;
	blowfish.Decrypt((unsigned char*)(LPTSTR)(LPCTSTR)strChgBuffer, (unsigned char*)strDecDataBuffer.GetBuffer(nEncodeDataLength), nEncodeDataLength);
	strDecDataBuffer.ReleaseBuffer();
	return (LPCTSTR)strDecDataBuffer;*/
	return "";
}

std::string BlowFish2AyersEncoding(string orgTxt, string key, BOOL bBase64)
{
	/*CString strAyersData("");
	int nAyersData = CBlowFish::CompressBlowfish(orgTxt.c_str(), strAyersData.GetBuffer(orgTxt.length()));
	strAyersData.ReleaseBuffer();
	int nLenMsg = 0;
	memcpy(&nLenMsg, (LPCTSTR)strAyersData, 4);
	char* pBlowfish = new char[3 * nLenMsg + 1]();
	memset(pBlowfish, 0x00, 3 * nLenMsg + 1);

	char* pBuf = (char*)strAyersData.GetBuffer();
	int nLen = CBlowFish::UncompressBlowfish(pBuf + 4, pBlowfish, nLenMsg);
	if (CString(orgTxt.c_str()) != CString(pBlowfish))
	{
		MessageBox(NULL, "Error", "Error", MB_OK);
	}
	SAFE_DELETE_ARR(pBlowfish);
	return CBase64::Encode(string(strAyersData.GetBuffer(nAyersData + 4), nAyersData + 4));*/
	return "";
}

string BlowFishEncoding(string orgTxt, string key, BOOL bBase64)
{
	/*CBlowFish blowfish((unsigned char*)key.c_str(), key.length());
	CString strEncodeData;
	blowfish.Encrypt((unsigned char*)orgTxt.c_str(), (unsigned char*)strEncodeData.GetBuffer(3 * orgTxt.length()), orgTxt.length());
	strEncodeData.ReleaseBuffer();
	if (bBase64 == TRUE)
	{
		return CBase64::Encode(string(strEncodeData.GetBuffer(strEncodeData.GetLength()), strEncodeData.GetLength()));
	}
	return strEncodeData.GetBuffer(strEncodeData.GetLength());*/
	return "";
}


std::string Base64Ayers2Blowfish(string orgTxt, string key, BOOL bBase64)
{
	/*string str = CBase64::Decode(orgTxt);
	CString strChgBuffer("");
	int nLenMsg = 0;
	memcpy(&nLenMsg, (LPCTSTR)str.c_str(), 4);
	int nEncodeDataLength = CBlowFish::UncompressBlowfish((char*)str.c_str() + 4, strChgBuffer.GetBuffer(nLenMsg), nLenMsg);
	strChgBuffer.ReleaseBuffer();
	return strChgBuffer;*/
	return "";
}

string LogSystemTest(string orgTxt, string key, BOOL bBase64)
{
	static CLog log;
	log.Initialize("test.log", "",true);
	log.Info(orgTxt);

	CLuaPacket luaPack;
	luaPack.SetData("testKey", "testValue");
	auto str = luaPack.SerializeToString();
	CLuaPacket luaPack2;
	luaPack2.ParseFromString(str);
	auto value = luaPack2.GetData("testKey", "");

	return "Done";
}


int main()
{
	LogSystemTest("test", "", FALSE);
}
