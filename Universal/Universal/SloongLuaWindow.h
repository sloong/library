#pragma once
#include "SloongUniversal.h"
namespace Sloong
{
	namespace Universal
	{
		class CLua;
		class CLuaWindow
		{
		public:
			CLuaWindow();
			virtual ~CLuaWindow();

		public:
			void Initialize();
			void SetLuaContext();
			void ShowWindow();
			void Render();

		protected:
			CLua*	m_pLua;
			HWND	m_pWnd;
			CRect	m_rcWindow;
		};
	}
}