#pragma once

#ifdef SLOONGGRAPHICS_EXPORTS
#define SLOONGGRAPHICS_API __declspec(dllexport)
#else
#define SLOONGGRAPHICS_API __declspec(dllimport)
#endif


#define BITMAP_ID            0x4D42 // universal id for a bitmap
#define BITMAP_ATTR_LOADED   128

#include <ddraw.h>    // directX includes
#include "string/string.h"
#pragma comment(lib,"string.lib")
namespace Sloong
{
	using namespace Universal;
	namespace Graphics
	{
		enum BITMAP_LOAD_MODE
		{
			BITMAP_EXTRACT_MODE_ABS = 0,
			BITMAP_EXTRACT_MODE_CELL = 1,
		};

		class SLOONGGRAPHICS_API CBitmap
		{
		public:
			CBitmap();
			~CBitmap();

			HRESULT Initialize();
			HRESULT LoadBitmapFromFile(CString strFileName);

			LONG GetBitmapHeight();
			LONG GetBitmapWidth();
			int GetBiemapBPP();

			HRESULT FlipBitmap(int nNumLine, int nHeight);
			int Scan_Image_Bitmap(LPRECT rcBmp, LPDIRECTDRAWSURFACE7 lpdds, LPRECT rcSurface);
			int Scan_Image_Bitmap(LPDIRECTDRAWSURFACE7 lpdds, int cx, int cy);
			HRESULT Shutdown();

		public:
			BITMAPFILEHEADER m_stFileHeader;
			BITMAPINFOHEADER m_stInfoHeader;

			LPBYTE			 m_lpBuffer;

		};
	}
}
