#include <iostream>
#include "IUniversal.h"
#pragma comment(lib,"Universal.lib")
using namespace std;
using namespace Sloong::Universal;


int main()
{

	// Test IUniversal interface.
	IUniversal* pUniversal = NULL;
	pUniversal = IUniversal::Create();
	LPCTSTR str = pUniversal->HelloWorld();
	wcout << str << endl;

	// Create ILogSystem from IUniversal::CreateLogSystem interface.
	ILogSystem* pLog = NULL;
	pUniversal->CreateLogSystem(pUniversal, &pLog);
	pLog->WriteLine(TEXT("TestLog"));
	pLog->WriteLine(pUniversal->HelloWorld());
	SAFE_DELETE(pLog);

	
	return 0;
}