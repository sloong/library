// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//
#ifndef STDAFX_H
#define STDAFX_H

#include "targetver.h"

#include <string>
using namespace std;
#ifdef _WINDOWS
	#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
	// Windows ͷ�ļ�: 
	#include <windows.h>
	#include <tchar.h>
	#include <assert.h>
	#include <time.h>
	#include <stdio.h>
	#include <fstream>
	#include <windows.h>
	#include <winerror.h>
	#include <vector>
	#pragma comment(lib,"libeay32.lib")
	#pragma comment(lib,"lua52.lib")
#else
	#include <stdarg.h>
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <wchar.h>
	#include <vector>
    #include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <semaphore.h>
#endif
#include "univ.h"
#endif //STDAFX_H

