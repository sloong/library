// SLUniversal.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include "log.h"
#include "univ.h"
#include "version.h"
#include <assert.h>
#include "exception.h"
#ifndef _WINDOWS
#include <libgen.h>
#include <openssl/md5.h>
#include <stdarg.h>  
#include <sys/stat.h>  
//#include <libzip.h>
#define ACCESS access  
#define MKDIR(a) mkdir((a),0755)  
#else
#include <Wincrypt.h>
#include <direct.h>  
#include <io.h>  
#define ACCESS _access  
#define MKDIR(a) _mkdir((a))  
#endif // !_WINDOWS

using namespace std;
using namespace Sloong::Universal;

typedef map<int, string> MSGMAP;
MSGMAP g_MessageMap;


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




wstring CUniversal::Version()
{
	return VERSION_LEGALCOPYRIGHT;
}

bool g_bIsInst = false;

void CUniversal::CopyStringToPoint(LPSTR& lpTarget, LPCSTR lpFrom)
{
	SAFE_DELETE_ARR(lpTarget);
	int nLength = (int)strlen(lpFrom);
	lpTarget = new char[nLength + 1];
	assert(lpTarget);
	strncpy(lpTarget, lpFrom, nLength);
}

void CUniversal::CopyStringToPoint(LPWSTR& lpTarget, LPCWSTR lpFrom)
{
	SAFE_DELETE_ARR(lpTarget);
	int nLength = (int)wcslen(lpFrom);
	lpTarget = new wchar_t[nLength + 1];
	assert(lpTarget);
	wcsncpy(lpTarget, lpFrom, nLength);
}



string CUniversal::trim(const string& str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}

wstring CUniversal::trim(const wstring& str)
{
	wstring::size_type pos = str.find_first_not_of(' ');
	if (pos == string::npos)
	{
		return str;
	}
	wstring::size_type pos2 = str.find_last_not_of(' ');
	if (pos2 != string::npos)
	{
		return str.substr(pos, pos2 - pos + 1);
	}
	return str.substr(pos);
}

int CUniversal::splitString(const string& str, vector<string>& ret_, string sep /* = "," */)
{
    if (str.empty())
    {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

string CUniversal::replace(const string& str, const string& src, const string& dest)
{
    string ret;

    string::size_type pos_begin = 0;
    string::size_type pos       = str.find(src);
    while (pos != string::npos)
    {
        ret.append(str.data() + pos_begin, pos - pos_begin);
        ret += dest;
        pos_begin = pos + 1;
        pos       = str.find(src, pos_begin);
    }
    if (pos_begin < str.length())
    {
        ret.append(str.begin() + pos_begin, str.end());
    }
    return ret;
}


wstring CUniversal::replace(const wstring& str, const wstring& src, const wstring& dest)
{
	wstring ret;

	wstring::size_type pos_begin = 0;
	wstring::size_type pos = str.find(src);
	while (pos != string::npos)
	{
		ret.append(str.data() + pos_begin, pos - pos_begin);
		ret += dest;
		pos_begin = pos + 1;
		pos = str.find(src, pos_begin);
	}
	if (pos_begin < str.length())
	{
		ret.append(str.begin() + pos_begin, str.end());
	}
	return ret;
}

std::string Sloong::Universal::CUniversal::CheckFileDirectory(string filePath)
{
	if (filePath == "")
	{
		return "";
	}

	int iLen = filePath.size();
	char* pszDir = new char[iLen + 1];
	memset(pszDir, 0, iLen + 1);
	memcpy(pszDir, filePath.c_str(), iLen);
	string strDir;
	int iRet;
	// 创建中间目录  
	for (int i = 1; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';
			strDir = pszDir;

			//如果不存在,创建  
			iRet = ACCESS(pszDir, 0);
			if (iRet != 0)
			{
				iRet = MKDIR(pszDir);
				if (iRet != 0)
				{
					return pszDir;
				}
			}
			//支持linux,将所有\换成/  
			pszDir[i] = '/';
		}
	}

	SAFE_DELETE_ARR(pszDir);
	return strDir;
}

string Sloong::Universal::CUniversal::toansi(const wstring& str)
{
	string strResult;
	int nLen = (int)str.size();
	LPSTR szMulti = new char[nLen + 1];
	memset(szMulti, 0, nLen + 1);
	// use the c standard library function to convert
	wcstombs(szMulti, str.c_str(), nLen);
	strResult = szMulti;
	delete[] szMulti;
	return strResult;


}

wstring Sloong::Universal::CUniversal::toutf(const string& str)
{
	wstring strResult;
	int nLen = (int)str.size();
	LPWSTR strWide = new WCHAR[nLen + 1];
	memset(strWide, 0, sizeof(TCHAR)*(nLen + 1));
	mbstowcs(strWide,str.c_str(),nLen);
	strResult = strWide;
	delete[] strWide;
	return strResult;
}




string Sloong::Universal::CUniversal::Base64_Encoding(string str)
{
    int i = 0, slen = str.size();
    char* dest = new char[slen + 1];
    const char* src = str.c_str();
    for (i = 0; i < slen; i += 3, src += 3)
    { // Enc next 4 characters
        *(dest++) = base64encode_lut[(*src & 0xFC) >> 0x2];
        *(dest++) = base64encode_lut[(*src & 0x3) << 0x4 | (*(src + 1) & 0xF0) >> 0x4];
        *(dest++) = ((i + 1) < slen) ? base64encode_lut[(*(src + 1) & 0xF) << 0x2 | (*(src + 2) & 0xC0) >> 0x6] : '=';
        *(dest++) = ((i + 2) < slen) ? base64encode_lut[*(src + 2) & 0x3F] : '=';
    }
    *dest = '\0'; // Append terminator
    return dest;
}

string Sloong::Universal::CUniversal::Base64_Decoding(string str)
{
    int i = 0, slen = str.size();
    char* dest = new char[slen + 1];
    const char* src = str.c_str();
    for (i = 0; i < slen; i += 4, src += 4)
    { // Store next 4 chars in vars for faster access
        char c1 = base64decode_lut[*src], c2 = base64decode_lut[*(src + 1)], c3 = base64decode_lut[*(src + 2)], c4 = base64decode_lut[*(src + 3)];
        // Decode to 3 chars
        *(dest++) = (c1 & 0x3F) << 0x2 | (c2 & 0x30) >> 0x4;
        *(dest++) = (c3 != 64) ? ((c2 & 0xF) << 0x4 | (c3 & 0x3C) >> 0x2) : '\0';
        *(dest++) = (c4 != 64) ? ((c3 & 0x3) << 0x6 | c4 & 0x3F) : '\0';
    }
    *dest = '\0'; // Append terminator
    return dest;
}

#ifdef _WINDOWS
typedef struct
{    ULONG i[2];
    ULONG buf[4];
    unsigned char in[64];
    unsigned char digest[16];
} MD5_CTX;
typedef void (CALLBACK* MD5Init_Tpye)(MD5_CTX* context);
typedef void (CALLBACK* MD5Update_Tpye)(MD5_CTX* context,unsigned char* input,unsigned int inlen);
typedef void (CALLBACK* MD5Final_Tpye)(MD5_CTX* context);
#endif // _WINDOWS


string Sloong::Universal::CUniversal::MD5_Binary_Encoding(string str, unsigned char* md, bool bFile /*= false*/ )
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

    if ( bFile )
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
        unsigned char* src = new unsigned char[str.size()+1];
        memset( src, 0, str.size()+1);
        memcpy( src, str.c_str(), str.size());
        MD5Update(&md5_context, src, str.size());
        SAFE_DELETE_ARR(src)
    }
    MD5Final(&md5_context);

    for(int i = 0; i < 16; ++i)
	{
		md[i] = md5_context.digest[i];
	}
	FreeLibrary(hDLL);
#else
    if ( bFile )
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

string Sloong::Universal::CUniversal::MD5_Encoding(string str, bool bFile /*= false*/)
{
	unsigned char md[16];
    char tmp[3] = { '\0' }, md5buf[33] = { '\0' };
	MD5_Binary_Encoding(str,md,bFile);
	for (int i = 0; i < 16; i++)
    {
        sprintf(tmp, "%02X", md[i]);
        strcat(md5buf, tmp);
    }

	return md5buf;
}

std::string Sloong::Universal::CUniversal::Replace(const string& str, const string& src, const string& dest)
{
	string res = str;
	for (string::size_type pos(0); pos != string::npos; pos += dest.length())   
	{
		if ((pos = str.find(src, pos)) != string::npos)
			res.replace(pos, src.length(), dest);
		else   
			break;
	}
	return  res;
}


#ifndef _WINDOWS
/*
static int compressString(const char* apData, int auDataSize, char* apOutBuf, int auOutBufSize, int* apOutBufLen)
{
	int ret = -1;
	*apOutBufLen = 0;

	zip_t *za;
	zip_source_t *zs;
	zip_stat_t zst;
	struct stat st;
	zip_source_t *src;
	zip_error_t error;
	int err;


	do
	{
		src = zip_source_buffer_create(NULL, 0, 0, &error);
		if (src == NULL) {
			err = zip_error_code_zip(&error);
			errno = zip_error_code_system(&error);
			fprintf(stderr, "zip_source_buffer_create faild: %d\n", err);
			break;
		}

		za = zip_open_from_source(src, 1, &error);
		if (za == NULL) {
			err = zip_error_code_zip(&error);
			errno = zip_error_code_system(&error);
			fprintf(stderr, "zip_open_from_source faild: %d\n", err);
			break;
		}

		zip_source_keep(src);

		if ((zs = zip_source_buffer(za, apData, auDataSize, 0)) == NULL) {
			fprintf(stderr, "can't create zip_source from buffer: %s\n", zip_strerror(za));
			break;
		}

		if (zip_add(za, archive, zs) == -1) {
			fprintf(stderr, "can't add file '%s': %s\n", archive, zip_strerror(za));
			break;
		}

		if (zip_close(za) == -1) {
			fprintf(stderr, "can't close zip archive '%s': %s\n", archive, zip_strerror(za));
			break;
		}

		za = NULL;

		if (zip_source_stat(src, &zst) < 0) {
			fprintf(stderr, "zip_source_stat on buffer failed: %s\n", zip_error_strerror(zip_source_error(src)));
			break;
		}

		if (zst.size <= 0) {
			printf(" size error 000\n");
			break;
		}

		if (zst.size >= auOutBufSize) {
			printf(" size error 111\n");
			break;
		}

		if (zip_source_open(src) < 0) {
			if (zip_error_code_zip(zip_source_error(src)) == ZIP_ER_DELETED) {
				if (unlink(archive) < 0 && errno != ENOENT) {
					fprintf(stderr, "unlink failed: %s\n", strerror(errno));
					break;
				}
				break;
			}
			fprintf(stderr, "zip_source_open on buffer failed: %s\n", zip_error_strerror(zip_source_error(src)));
			break;
		}


		if (zip_source_read(src, apOutBuf, zst.size) < (zip_int64_t)zst.size) {
			fprintf(stderr, "zip_source_read on buffer failed: %s\n", zip_error_strerror(zip_source_error(src)));
			zip_source_close(src);
			break;
		}

		zip_source_close(src);
		*apOutBufLen = (int)(zst.size);
		ret = 0;

		//saveZip(apOutBuf,*apOutBufLen );

	} while (0);

	if (NULL != src)
	{
		zip_source_free(src);
		src = NULL;
	}

	if (NULL != za)
	{
		zip_close(za);
		za = NULL;
	}


	return ret;
}


static int uncompressString(const char* apData, int auDataSize, char* apOutBuf, int auOutBufSize, int* apOutBufLen)
{
	int ret = -1;

	*apOutBufLen = 0;
	zip_error_t error;
	int err = 0;
	char* buf = apOutBuf;
	int   totalSize = 0;
	zip_int64_t n = 0;
	zip_source_t *src = NULL;
	zip_t *za = NULL;
	struct zip_file *f = NULL;


	do
	{
		zip_error_init(&error);

		// create source from buffer 
		if ((src = zip_source_buffer_create(apData, auDataSize, 1, &error)) == NULL) {
			fprintf(stderr, "can't create source: %s\n", zip_error_strerror(&error));
			zip_error_fini(&error);
			break;
		}

		// open zip archive from source 
		if ((za = zip_open_from_source(src, 0, &error)) == NULL) {
			fprintf(stderr, "can't open zip from source: %s\n", zip_error_strerror(&error));
			zip_error_fini(&error);
			break;
		}


		zip_error_fini(&error);
		zip_source_keep(src);

		zip_int64_t  c = zip_get_num_entries(za, ZIP_FL_UNCHANGED);
		if (c != 1)
		{
			printf("zip_get_num_entries 0 \n");
			break;
		}

		const char * name = zip_get_name(za, 0, ZIP_FL_ENC_GUESS);
		if (NULL == name)
		{
			printf("zip_get_name 0 \n");
			break;
		}

		f = zip_fopen(za, name, 0);
		if (NULL == f)
		{
			printf("zip_fopen 0 \n");
			break;
		}

		if (auOutBufSize < 4096)
		{
			printf("auOutBufSize < 4096 \n");
			break;
		}

		totalSize = 0;
		while (totalSize < auOutBufSize)
		{
			buf = apOutBuf + totalSize;
			n = zip_fread(f, buf, 4096);
			if (n <= 0)
			{
				break;
			}

			totalSize += n;
		}

		if (totalSize >= auOutBufSize)
		{
			printf("totalSize too big \n");
			break;
		}

		*apOutBufLen = totalSize;
		ret = 0;

	} while (0);


	if (NULL != f)
	{
		zip_fclose(f);
		f = NULL;
	}

	if (NULL != za)
	{
		//lt-in-memory: free(): invalid pointer: 0x00007fff9c75c6d0 ***
		//zip_close(za);
		za = NULL;
	}

	if (NULL != src)
	{
		zip_source_free(src);
		src = NULL;
	}

	return ret;
}
*/
#else

// Remarks:
//		Format the windows error message
wstring CUniversal::FormatWindowsErrorMessage( DWORD dwErrCode)
{
	wstring strError;
	TCHAR szErr[1024] = { 0 };
	DWORD systemLocale = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	DWORD dwLength = 0;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrCode, systemLocale, szErr, 1024, NULL);
	strError = szErr;
	return strError;
}




// CSize
inline CSize::CSize() throw()
{
	cx = 0;
	cy = 0;
}

inline CSize::CSize(
	_In_ int initCX,
	_In_ int initCY) throw()
{
	cx = initCX;
	cy = initCY;
}

inline CSize::CSize(_In_ SIZE initSize) throw()
{
	*(SIZE*)this = initSize;
}

inline CSize::CSize(_In_ POINT initPt) throw()
{
	*(POINT*)this = initPt;
}

inline CSize::CSize(_In_ DWORD dwSize) throw()
{
	cx = (short)LOWORD(dwSize);
	cy = (short)HIWORD(dwSize);
}

inline BOOL CSize::operator==(_In_ SIZE size) const throw()
{
	return (cx == size.cx && cy == size.cy);
}

inline BOOL CSize::operator!=(_In_ SIZE size) const throw()
{
	return (cx != size.cx || cy != size.cy);
}

inline void CSize::operator+=(_In_ SIZE size) throw()
{
	cx += size.cx;
	cy += size.cy;
}

inline void CSize::operator-=(_In_ SIZE size) throw()
{
	cx -= size.cx;
	cy -= size.cy;
}

inline void CSize::SetSize(
	_In_ int CX,
	_In_ int CY) throw()
{
	cx = CX;
	cy = CY;
}

inline CSize CSize::operator+(_In_ SIZE size) const throw()
{
	return CSize(cx + size.cx, cy + size.cy);
}

inline CSize CSize::operator-(_In_ SIZE size) const throw()
{
	return CSize(cx - size.cx, cy - size.cy);
}

inline CSize CSize::operator-() const throw()
{
	return CSize(-cx, -cy);
}

inline CPoint CSize::operator+(_In_ POINT point) const throw()
{
	return CPoint(cx + point.x, cy + point.y);
}

inline CPoint CSize::operator-(_In_ POINT point) const throw()
{
	return CPoint(cx - point.x, cy - point.y);
}


// CPoint
inline CPoint::CPoint() throw()
{
	x = 0;
	y = 0;
}

inline CPoint::CPoint(
	_In_ int initX,
	_In_ int initY) throw()
{
	x = initX;
	y = initY;
}

inline CPoint::CPoint(_In_ POINT initPt) throw()
{
	*(POINT*)this = initPt;
}

inline CPoint::CPoint(_In_ SIZE initSize) throw()
{
	*(SIZE*)this = initSize;
}

inline CPoint::CPoint(_In_ LPARAM dwPoint) throw()
{
	x = (short)LOWORD(dwPoint);
	y = (short)HIWORD(dwPoint);
}

inline void CPoint::Offset(
	_In_ int xOffset,
	_In_ int yOffset) throw()
{
	x += xOffset;
	y += yOffset;
}

inline void CPoint::Offset(_In_ POINT point) throw()
{
	x += point.x;
	y += point.y;
}

inline void CPoint::Offset(_In_ SIZE size) throw()
{
	x += size.cx;
	y += size.cy;
}

inline void CPoint::SetPoint(
	_In_ int X,
	_In_ int Y) throw()
{
	x = X;
	y = Y;
}

inline BOOL CPoint::operator==(_In_ POINT point) const throw()
{
	return (x == point.x && y == point.y);
}

inline BOOL CPoint::operator!=(_In_ POINT point) const throw()
{
	return (x != point.x || y != point.y);
}

inline void CPoint::operator+=(_In_ SIZE size) throw()
{
	x += size.cx;
	y += size.cy;
}

inline void CPoint::operator-=(_In_ SIZE size) throw()
{
	x -= size.cx;
	y -= size.cy;
}

inline void CPoint::operator+=(_In_ POINT point) throw()
{
	x += point.x;
	y += point.y;
}

inline void CPoint::operator-=(_In_ POINT point) throw()
{
	x -= point.x;
	y -= point.y;
}

inline CPoint CPoint::operator+(_In_ SIZE size) const throw()
{
	return CPoint(x + size.cx, y + size.cy);
}

inline CPoint CPoint::operator-(_In_ SIZE size) const throw()
{
	return CPoint(x - size.cx, y - size.cy);
}

inline CPoint CPoint::operator-() const throw()
{
	return CPoint(-x, -y);
}

inline CPoint CPoint::operator+(_In_ POINT point) const throw()
{
	return CPoint(x + point.x, y + point.y);
}

inline CSize CPoint::operator-(_In_ POINT point) const throw()
{
	return CSize(x - point.x, y - point.y);
}




inline CRect CSize::operator+(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) + *this;
}

inline CRect CSize::operator-(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) - *this;
}



inline CRect CPoint::operator+(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) + *this;
}

inline CRect CPoint::operator-(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) - *this;
}


// CRect
inline CRect::CRect() throw()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

inline CRect::CRect(
	_In_ int l,
	_In_ int t,
	_In_ int r,
	_In_ int b) throw()
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

inline CRect::CRect(_In_ const RECT& srcRect) throw()
{
	::CopyRect(this, &srcRect);
}

inline CRect::CRect(_In_ LPCRECT lpSrcRect) throw()
{
	::CopyRect(this, lpSrcRect);
}

inline CRect::CRect(
	_In_ POINT point,
	_In_ SIZE size) throw()
{
	right = (left = point.x) + size.cx;
	bottom = (top = point.y) + size.cy;
}

inline CRect::CRect(
	_In_ POINT topLeft,
	_In_ POINT bottomRight) throw()
{
	left = topLeft.x;
	top = topLeft.y;
	right = bottomRight.x;
	bottom = bottomRight.y;
}

inline int CRect::Width() const throw()
{
	return right - left;
}

inline int CRect::Height() const throw()
{
	return bottom - top;
}

inline CSize CRect::Size() const throw()
{
	return CSize(right - left, bottom - top);
}

inline CPoint& CRect::TopLeft() throw()
{
	return *((CPoint*)this);
}

inline CPoint& CRect::BottomRight() throw()
{
	return *((CPoint*)this + 1);
}

inline const CPoint& CRect::TopLeft() const throw()
{
	return *((CPoint*)this);
}

inline const CPoint& CRect::BottomRight() const throw()
{
	return *((CPoint*)this + 1);
}

inline CPoint CRect::CenterPoint() const throw()
{
	return CPoint((left + right) / 2, (top + bottom) / 2);
}

inline void CRect::SwapLeftRight() throw()
{
	SwapLeftRight(LPRECT(this));
}

inline void WINAPI CRect::SwapLeftRight(_Inout_ LPRECT lpRect) throw()
{
	LONG temp = lpRect->left;
	lpRect->left = lpRect->right;
	lpRect->right = temp;
}

inline CRect::operator LPRECT() throw()
{
	return this;
}

inline CRect::operator LPCRECT() const throw()
{
	return this;
}

inline BOOL CRect::IsRectEmpty() const throw()
{
	return ::IsRectEmpty(this);
}

inline BOOL CRect::IsRectNull() const throw()
{
	return (left == 0 && right == 0 && top == 0 && bottom == 0);
}

inline BOOL CRect::PtInRect(_In_ POINT point) const throw()
{
	return ::PtInRect(this, point);
}

inline void CRect::SetRect(
	_In_ int x1,
	_In_ int y1,
	_In_ int x2,
	_In_ int y2) throw()
{
	::SetRect(this, x1, y1, x2, y2);
}

inline void CRect::SetRect(
	_In_ POINT topLeft,
	_In_ POINT bottomRight) throw()
{
	::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

inline void CRect::SetRectEmpty() throw()
{
	::SetRectEmpty(this);
}

inline void CRect::CopyRect(_In_ LPCRECT lpSrcRect) throw()
{
	::CopyRect(this, lpSrcRect);
}

inline BOOL CRect::EqualRect(_In_ LPCRECT lpRect) const throw()
{
	return ::EqualRect(this, lpRect);
}

inline void CRect::InflateRect(
	_In_ int x,
	_In_ int y) throw()
{
	::InflateRect(this, x, y);
}

inline void CRect::InflateRect(_In_ SIZE size) throw()
{
	::InflateRect(this, size.cx, size.cy);
}

inline void CRect::DeflateRect(
	_In_ int x,
	_In_ int y) throw()
{
	::InflateRect(this, -x, -y);
}

inline void CRect::DeflateRect(_In_ SIZE size) throw()
{
	::InflateRect(this, -size.cx, -size.cy);
}

inline void CRect::OffsetRect(
	_In_ int x,
	_In_ int y) throw()
{
	::OffsetRect(this, x, y);
}

inline void CRect::OffsetRect(_In_ POINT point) throw()
{
	::OffsetRect(this, point.x, point.y);
}

inline void CRect::OffsetRect(_In_ SIZE size) throw()
{
	::OffsetRect(this, size.cx, size.cy);
}

inline void CRect::MoveToY(_In_ int y) throw()
{
	bottom = Height() + y;
	top = y;
}

inline void CRect::MoveToX(_In_ int x) throw()
{
	right = Width() + x;
	left = x;
}

inline void CRect::MoveToXY(
	_In_ int x,
	_In_ int y) throw()
{
	MoveToX(x);
	MoveToY(y);
}

inline void CRect::MoveToXY(_In_ POINT pt) throw()
{
	MoveToX(pt.x);
	MoveToY(pt.y);
}

inline BOOL CRect::IntersectRect(
	_In_ LPCRECT lpRect1,
	_In_ LPCRECT lpRect2) throw()
{
	return ::IntersectRect(this, lpRect1, lpRect2);
}

inline BOOL CRect::UnionRect(
	_In_ LPCRECT lpRect1,
	_In_ LPCRECT lpRect2) throw()
{
	return ::UnionRect(this, lpRect1, lpRect2);
}

inline void CRect::operator=(_In_ const RECT& srcRect) throw()
{
	::CopyRect(this, &srcRect);
}

inline BOOL CRect::operator==(_In_ const RECT& rect) const throw()
{
	return ::EqualRect(this, &rect);
}

inline BOOL CRect::operator!=(_In_ const RECT& rect) const throw()
{
	return !::EqualRect(this, &rect);
}

inline void CRect::operator+=(_In_ POINT point) throw()
{
	::OffsetRect(this, point.x, point.y);
}

inline void CRect::operator+=(_In_ SIZE size) throw()
{
	::OffsetRect(this, size.cx, size.cy);
}

inline void CRect::operator+=(_In_ LPCRECT lpRect) throw()
{
	InflateRect(lpRect);
}

inline void CRect::operator-=(_In_ POINT point) throw()
{
	::OffsetRect(this, -point.x, -point.y);
}

inline void CRect::operator-=(_In_ SIZE size) throw()
{
	::OffsetRect(this, -size.cx, -size.cy);
}

inline void CRect::operator-=(_In_ LPCRECT lpRect) throw()
{
	DeflateRect(lpRect);
}

inline void CRect::operator&=(_In_ const RECT& rect) throw()
{
	::IntersectRect(this, this, &rect);
}

inline void CRect::operator|=(_In_ const RECT& rect) throw()
{
	::UnionRect(this, this, &rect);
}

inline CRect CRect::operator+(_In_ POINT pt) const throw()
{
	CRect rect(*this);
	::OffsetRect(&rect, pt.x, pt.y);
	return rect;
}

inline CRect CRect::operator-(_In_ POINT pt) const throw()
{
	CRect rect(*this);
	::OffsetRect(&rect, -pt.x, -pt.y);
	return rect;
}

inline CRect CRect::operator+(_In_ SIZE size) const throw()
{
	CRect rect(*this);
	::OffsetRect(&rect, size.cx, size.cy);
	return rect;
}

inline CRect CRect::operator-(_In_ SIZE size) const throw()
{
	CRect rect(*this);
	::OffsetRect(&rect, -size.cx, -size.cy);
	return rect;
}

inline CRect CRect::operator+(_In_ LPCRECT lpRect) const throw()
{
	CRect rect(this);
	rect.InflateRect(lpRect);
	return rect;
}

inline CRect CRect::operator-(_In_ LPCRECT lpRect) const throw()
{
	CRect rect(this);
	rect.DeflateRect(lpRect);
	return rect;
}

inline CRect CRect::operator&(_In_ const RECT& rect2) const throw()
{
	CRect rect;
	::IntersectRect(&rect, this, &rect2);
	return rect;
}

inline CRect CRect::operator|(_In_ const RECT& rect2) const throw()
{
	CRect rect;
	::UnionRect(&rect, this, &rect2);
	return rect;
}

inline BOOL CRect::SubtractRect(
	_In_ LPCRECT lpRectSrc1,
	_In_ LPCRECT lpRectSrc2) throw()
{
	return ::SubtractRect(this, lpRectSrc1, lpRectSrc2);
}

inline void CRect::NormalizeRect() throw()
{
	int nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

inline void CRect::InflateRect(_In_ LPCRECT lpRect) throw()
{
	left -= lpRect->left;
	top -= lpRect->top;
	right += lpRect->right;
	bottom += lpRect->bottom;
}

inline void CRect::InflateRect(
	_In_ int l,
	_In_ int t,
	_In_ int r,
	_In_ int b) throw()
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

inline void CRect::DeflateRect(_In_ LPCRECT lpRect) throw()
{
	left += lpRect->left;
	top += lpRect->top;
	right -= lpRect->right;
	bottom -= lpRect->bottom;
}

inline void CRect::DeflateRect(
	_In_ int l,
	_In_ int t,
	_In_ int r,
	_In_ int b) throw()
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

inline CRect CRect::MulDiv(
	_In_ int nMultiplier,
	_In_ int nDivisor) const throw()
{
	return CRect(
		::MulDiv(left, nMultiplier, nDivisor),
		::MulDiv(top, nMultiplier, nDivisor),
		::MulDiv(right, nMultiplier, nDivisor),
		::MulDiv(bottom, nMultiplier, nDivisor));
}

#endif // _WINDOWS

