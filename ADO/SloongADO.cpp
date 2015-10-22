// SloongADO.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SloongADO.h"


// This is an example of an exported variable
SLOONGADO_API int nSloongADO=0;

// This is an example of an exported function.
SLOONGADO_API int fnSloongADO(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see SloongADO.h for the class definition
CSloongADO::CSloongADO()
{
	return;
}
