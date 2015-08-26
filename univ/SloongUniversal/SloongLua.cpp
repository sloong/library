#include "stdafx.h"
#include "SloongLua.h"
#include "SloongException.h"
using namespace Sloong::Universal;

typedef int(*LuaFunc)(lua_State* pLuaState);

CLua::CLua()
{
	m_pErrorHandler = NULL;

	m_pScriptContext = luaL_newstate();
	luaL_openlibs(m_pScriptContext);
	m_pMutex = CreateMutex(NULL, FALSE, _T("SloongLuaMutex"));
}

CLua::~CLua()
{
	if (m_pScriptContext)
		lua_close(m_pScriptContext);

	CloseHandle(m_pMutex);
}

static wstring findScript(wstring strFullName)
{
	string strFileName = CUniversal::ToString(strFullName);
	
	FILE* fFind;

	char szDrive[MAX_PATH];
	char szDir[MAX_PATH];
	char szFileName[MAX_PATH];
	char szExtension[MAX_PATH];

	_splitpath_s(strFileName.c_str(), szDrive, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, szExtension, MAX_PATH);

	string strTestFile = (string)szDrive + szDir + ("Scripts\\") + szFileName + (".LUB");
	fopen_s(&fFind,strTestFile.c_str(), "r");
	if (!fFind)
	{
		strTestFile = (string)szDrive + szDir + ("Scripts\\") + szFileName + (".LUA");
		fopen_s(&fFind, strTestFile.c_str(), "r");
	}

	if (!fFind)
	{
		strTestFile = (string)szDrive + szDir + szFileName + (".LUB");
		fopen_s(&fFind, strTestFile.c_str(), "r");
	}

	if (!fFind)
	{
		strTestFile = (string)szDrive + szDir + szFileName + (".LUA");
		fopen_s(&fFind, strTestFile.c_str(), "r");
	}

	if (fFind)
	{
		fclose(fFind);
	}

	return CUniversal::ToWString(strTestFile);
}

bool CLua::RunScript(wstring strFileName)
{
	WaitForSingleObject(m_pMutex, INFINITE);

	wstring strFullName = findScript(strFileName);

	if ( 0 != luaL_loadfile(m_pScriptContext, CUniversal::ToString(strFullName).c_str()))
	{
		HandlerError(_T("Load Script"), strFullName.c_str());
		ReleaseMutex(m_pMutex);
		return false;
	}

	if ( 0 != lua_pcall(m_pScriptContext,0,LUA_MULTRET,0))
	{
		HandlerError(_T("Run Script"), strFullName.c_str());
		ReleaseMutex(m_pMutex);
		return false;
	}
	ReleaseMutex(m_pMutex);
	return true;
}


bool CLua::RunBuffer( LPCSTR pBuffer,size_t sz)
{
	if (0 != luaL_loadbuffer(m_pScriptContext, (LPCSTR)pBuffer, sz, NULL))
	{
		string str(pBuffer);
		HandlerError(_T("Load Buffer"), CUniversal::ToWString(str).c_str());
		return false;
	}

	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		string str(pBuffer);
		HandlerError(_T("Run Buffer"), CUniversal::ToWString(str).c_str());
		return false;
	}
	return true;
}

bool CLua::RunString(wstring strCommand)
{
	WaitForSingleObject(m_pMutex, INFINITE);
	if (0 != luaL_loadstring(m_pScriptContext, CUniversal::ToString(strCommand).c_str()))
	{
		HandlerError(_T("String Load"), strCommand.c_str());
		ReleaseMutex(m_pMutex);
		return false;
	}

	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		HandlerError(_T("Run String"), strCommand.c_str());
		ReleaseMutex(m_pMutex);
		return false;
	}

	ReleaseMutex(m_pMutex);
	return true;
}

wstring CLua::GetErrorString()
{
	string strError(luaL_checkstring(m_pScriptContext, -1));

	return CUniversal::ToWString(strError);
}


bool CLua::AddFunction( wstring pFunctionName, LuaFunctionType pFunction)
{
	LuaFunc pFunc = (LuaFunc)pFunction;
	lua_register(m_pScriptContext, CUniversal::ToString(pFunctionName).c_str(), pFunc);
	return true;
}


wstring CLua::GetStringArgument(int num, wstring pDefault)
{
	auto str = luaL_optstring(m_pScriptContext, num, CUniversal::ToString(pDefault).c_str());

	return CUniversal::ToWString(str);
}

double CLua::GetNumberArgument(int num, double dDefault)
{
	return luaL_optnumber(m_pScriptContext, num, dDefault);
}

void CLua::PushString(wstring pString)
{
	lua_pushstring(m_pScriptContext, CUniversal::ToString(pString).c_str());
}

void CLua::PushNumber(double value)
{
	lua_pushnumber(m_pScriptContext, value);
}

bool CLua::RunFunction(wstring strFunctionName, wstring args)
{
	auto str = CUniversal::Format(L"%s(%s)", strFunctionName.c_str(), args.c_str());
	return RunString(str);
}

void CLua::HandlerError(wstring strErrorType, wstring strCmd)
{
	if (m_pErrorHandler)
	{
		auto str = CUniversal::Format(L"\n Error - %s:\n %s\n Error Message:%s", strErrorType.c_str(), strCmd.c_str(), GetErrorString().c_str());
		m_pErrorHandler(str.c_str());
	}
}

map<wstring, wstring> CLua::GetTableParam(int index)
{
	auto L = m_pScriptContext;
	map<wstring, wstring> data;
	lua_pushnil(L);
	// ���ڵ�ջ��-1 => nil; index => table
	if ( index >= lua_gettop(L))
	{
		throw CException(_T("The index is too big."));
	}

	while (lua_next(L, index))
	{
		// ���ڵ�ջ��-1 => value; -2 => key; index => table
		// ����һ�� key ��ջ����Ȼ������� lua_tostring �Ͳ���ı�ԭʼ�� key ֵ��
		lua_pushvalue(L, -2);
		// ���ڵ�ջ��-1 => key; -2 => value; -3 => key; index => table

		wstring key = CUniversal::ToWString(lua_tostring(L, -1));
		wstring value = CUniversal::ToWString(lua_tostring(L, -2));

		data[key] = value;
		// ���� value �Ϳ����� key������ԭʼ�� key ��Ϊ��һ�� lua_next �Ĳ���
		lua_pop(L, 2);
		// ���ڵ�ջ��-1 => key; index => table
	}
	// ���ڵ�ջ��index => table ����� lua_next ���� 0 ��ʱ�����Ѿ�����һ�����µ� key �������ˣ�
	// ����ջ�Ѿ��ָ��������������ʱ��״̬
	return data;

}

LuaType CLua::CheckType(int index)
{
	int nType = lua_type(m_pScriptContext, index);
	return (LuaType)nType;
}

double CLua::StringToNumber(wstring string)
{
	lua_stringtonumber(m_pScriptContext, CUniversal::ToString(string).c_str());
	
	return lua_tonumber(m_pScriptContext, -1);
}

inline lua_State* CLua::GetScriptContext()
{
	return m_pScriptContext;
}

inline void CLua::SetErrorHandle(void(*pErrHandler)(wstring strError))
{
	m_pErrorHandler = pErrHandler;
}

void CLua::InitializeWindow(HWND hWnd)
{

}

void Sloong::Universal::CLua::ShowLuaWindow()
{

}

void Sloong::Universal::CLua::RenderLuaWindow()
{

}
