#include <iostream>
#include "..\Universal\IUniversal.h"
#pragma comment(lib,"..\\Debug\\Universal.lib")
using namespace std;
using namespace SoaringLoong;


int main()
{
	CoInitialize(NULL);
	IUniversal* pTest = NULL;
	CreateUniversal((LPVOID*)&pTest);
	ILogSystem* pLog = NULL;
	CreateLogSystem((LPVOID*)&pLog);
	pLog->WriteLog(TEXT("TestLog"));
	LPCTSTR str = pTest->HelloWorld();
	wcout << str << endl;
	pTest->Release();	
	CoUninitialize();	
	
	return 0;
}