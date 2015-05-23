#pragma once

#define LuaRes extern "C" int

#include "SloongUniversal.h"
namespace Sloong
{
	namespace Universal
	{
		extern "C" {
#include "..\\SloongLua\\lua-5.3.0\\src\\lua.h"
#include "..\\SloongLua\\lua-5.3.0\\src\\lualib.h"
#include "..\\SloongLua\\lua-5.3.0\\src\\lauxlib.h"
		}
#pragma comment(lib,"SloongLua.lib")

		typedef int(*LuaFunctionType)(LPVOID pLuaState);

		struct LuaFunctionRegistr
		{
			LPCTSTR strFunctionName;
			LuaFunctionType pFunction;
		};

		enum LuaType
		{
			TNONE = -1,

			TNIL = 0,
			TBOOLEAN = 1,
			TLIGHTUSERDATA = 2,
			TNUMBER = 3,
			TSTRING = 4,
			TTABLE = 5,
			TFUNCTION = 6,
			TUSERDATA = 7,
			TTHREAD = 8,
			NUMTAGS = 9,
		};


		class UNIVERSAL_API CLua
		{
		public:
			CLua();
			virtual ~CLua();

			bool	RunScript(wstring strFileName);
			bool	RunBuffer(LPCSTR pBuffer, size_t sz);
			bool	RunString(wstring strCommand);
			bool	RunFunction(wstring strFunctionName, wstring args);
			wstring	GetErrorString();
			void	HandlerError(wstring strErrorType, wstring strCmd);
			bool	AddFunction(wstring strFunctionName, LuaFunctionType pFunction);
			wstring	GetStringArgument(int nNum, wstring pDefault = L"");
			double	GetNumberArgument(int nNum, double pDefault = -1.0f);
			void	PushString(wstring strString);
			void	PushNumber(double dValue);
			void	SetErrorHandle(void(*pErrHandler)(wstring strError));
			lua_State*	GetScriptContext();
			map<wstring, wstring> GetTableParam(int index);
			LuaType	CheckType(int index);
			double  StringToNumber(wstring string);
			void	InitializeWindow(HWND hWnd);
			void	ShowLuaWindow();
			void	RenderLuaWindow();

		private:
			lua_State *m_pScriptContext;
			void(*m_pErrorHandler)(wstring strError);
			HANDLE	m_pMutex;
		};

	}
	
}