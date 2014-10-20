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
	pTest->CreateLogSystem(pTest, pLog);
	ILinkList* pLinkList = NULL;
	pTest->CreateLinkList(pLinkList);
	pLog->WriteLine(TEXT("TestLog"));
	LPCTSTR str = pTest->HelloWorld();
	wcout << str << endl;
	pLog->Release();
	pTest->Release();	
	
	CoUninitialize();	
	
	return 0;
}