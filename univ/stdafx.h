// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//
#ifndef STDAFX_H
#define STDAFX_H

#include "targetver.h"

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
#else
	#include <stdarg.h>
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <wchar.h>
#endif
#include "univ.h"

#endif //STDAFX_H
