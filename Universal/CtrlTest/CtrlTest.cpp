#include <iostream>
#include "..\Universal\IUniversal.h"
#pragma comment(lib,"..\\Debug\\Universal.lib")
using namespace std;
using namespace SoaringLoong;


int main()
{
	CoInitialize(NULL);
	IUniversal* pTest = NULL;
	CreateComper((LPVOID*)&pTest);
	LPCTSTR str = pTest->HelloWorld();
	wcout << str << endl;
	pTest->Release();	
	//pUnknown->Release();
	CoUninitialize();	
	
	return 0;
}