// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SLOONGADO_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SLOONGADO_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SLOONGADO_EXPORTS
#define SLOONGADO_API __declspec(dllexport)
#else
#define SLOONGADO_API __declspec(dllimport)
#endif

// This class is exported from the SloongADO.dll
class SLOONGADO_API CSloongADO {
public:
	CSloongADO(void);
	// TODO: add your methods here.
};

extern SLOONGADO_API int nSloongADO;

SLOONGADO_API int fnSloongADO(void);
