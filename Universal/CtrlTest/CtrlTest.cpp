#include <iostream>
#include "IUniversal.h"
#pragma comment(lib,"Universal.lib")
using namespace std;
using namespace SoaringLoong;


int main()
{
	CoInitialize(NULL);

	// Test IUniversal interface.
	IUniversal* pUniversal = NULL;
	CreateUniversal((LPVOID*)&pUniversal);
	LPCTSTR str = pUniversal->HelloWorld();
	wcout << str << endl;

	// Create ILogSystem from IUniversal::CreateLogSystem interface.
	ILogSystem* pLog = NULL;
	pUniversal->CreateLogSystem(pUniversal, &pLog);
	pLog->WriteLine(TEXT("TestLog"));
	pLog->WriteLine(pUniversal->HelloWorld());
	pLog->Release();

	// Create ILogSystem by CoCreateInstance interface.
	ILogSystem* pLog2 = NULL;
	CoCreateInstance(IID_IUniversal, NULL, CLSCTX_INPROC_SERVER, IID_ILogSystem, (LPVOID*)&pLog2);
	pLog2->Initialize(pUniversal, TEXT("Log2.Log"));
	pLog2->WriteLine(TEXT("TEST2"));
	pLog->WriteLine(pUniversal->HelloWorld());
	pLog2->Release();
	
	pUniversal->Release();
	
	CoUninitialize();	
	
	return 0;
}