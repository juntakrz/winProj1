#include "main.h"

HDC mainDC;
HDC bDC;
HWND xWnd = mainWnd;

void PaintMainWindow(HWND hWnd, HDC hdc) {

	//PAINTSTRUCT ps;			//define paint structure
	//HDC hdc = BeginPaint(hWnd, &ps);				//handle device context

	HPEN cPen = CreatePen(PS_SOLID, 1, RGB(120, 0, 0));

	HGDIOBJ oldPen = SelectObject(hdc, cPen);
	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));	//make brush transparent

	BITMAP bmpObj;								//create BITMAP from handle to bitmap
	GetObject(hbBkBMP, sizeof(BITMAP), &bmpObj);	// ^^^^
	HDC compatDC = CreateCompatibleDC(hdc);			//create compatible device context to use the bitmap in

	SelectObject(compatDC, hbBkBMP);					//select bitmap to use with the compatible DC


	//stretch from DC2 to DC1 with offsets and new size (in this case fill the window)

	StretchBlt(hdc, 0, 0, clRect.right + 0, clRect.bottom + 0, compatDC, 0, 0, bmpObj.bmWidth, bmpObj.bmHeight, SRCCOPY);
	Rectangle(hdc, 0, 0, clRect.right, clRect.bottom);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);

	DeleteDC(compatDC);
	DeleteObject(cPen);

}

void PaintMonitorData(HWND hWnd, HDC hdc, PAINTSTRUCT ps) {

	HWND c_hWnd = GetDlgItem(mainWnd, IDC_TEXT02);
	//PAINTSTRUCT ps;
	hdc = BeginPaint(c_hWnd, &ps);
	BeginPaint(c_hWnd, &ps);

	HPEN cPen = CreatePen(PS_SOLID, 1, RGB(120, 0, 0));

	HFONT cFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Tahoma");
	POINT p = { 0 };
	MapWindowPoints(c_hWnd, GetParent(c_hWnd), &p, 1);
	RECT rect = { p.x, p.y, p.x+605, p.y+25 };

	SetBkMode(hdc, TRANSPARENT);			//text setup
	SetTextColor(hdc, RGB(160, 0, 0));
	
	HGDIOBJ oldFont = SelectObject(hdc, cFont);
	HGDIOBJ oldPen = SelectObject(hdc, cPen);
	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

	DrawTextW(hdc, GetMonitorData().c_str(), -1, &rect, DT_CENTER | DT_NOCLIP);
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	SelectObject(hdc, oldFont);
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);

	DeleteObject(cFont);
	DeleteObject(cPen);
	EndPaint(c_hWnd, &ps);
}

void PaintScreenTitle(HWND hWnd, HDC hdc, PAINTSTRUCT ps) {

	HWND c_hWnd = GetDlgItem(hWnd, IDC_TEXT01);
	//PAINTSTRUCT ps;
	//HDC hdc = BeginPaint(c_hWnd, &ps);
	BeginPaint(c_hWnd, &ps);
	//HPEN cPen = CreatePen(PS_SOLID, 1, RGB(120, 0, 0));

	HFONT cFont = CreateFont(16, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, APP_DEFFONT);

	POINT p = { 0 };
	MapWindowPoints(c_hWnd, GetParent(c_hWnd), &p, 1);
	RECT rect = { p.x, p.y, 605, 25 };

	SetBkMode(hdc, TRANSPARENT);			//text setup
	SetTextColor(hdc, RGB(255, 255, 0));

	HGDIOBJ oldFont = SelectObject(hdc, cFont);
	//HGDIOBJ oldPen = SelectObject(hdc, cPen);
	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

	DrawTextW(hdc, L"Red Alert 2 Configuration Tool", -1, &rect, DT_LEFT| DT_NOCLIP);

	SelectObject(hdc, oldFont);
	//SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(cFont);
	//DeleteObject(cPen);
	EndPaint(c_hWnd, &ps);

}

void PaintButton(HWND hWnd, HDC hdc, PAINTSTRUCT ps, DWORD IDC, std::wstring wsTitle = {}) {

	HWND c_hWnd = GetDlgItem(hWnd, IDC);
	//PAINTSTRUCT ps;
	//HDC hdc = BeginPaint(c_hWnd, &ps);
	//HDC hdc = GetDC(c_hWnd);
	BeginPaint(c_hWnd, &ps);
	HDC hBufferDC = CreateCompatibleDC(hdc);

	//determine the correct position of the painted control
	POINT p = { 0 };
	MapWindowPoints(c_hWnd, GetParent(c_hWnd), &p, 1);

	//wchar_t debug[20];
	//wsprintfW(debug, L"x: %d, y: %d", p.x, p.y);
	//MessageBoxW(nullptr, L"A what?", L"DEBUG", MB_OK);

	HBITMAP hbCurBitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDB_BITMAP2));
	BITMAP bCurBitmap;
	GetObjectW(hbCurBitmap, sizeof(BITMAP), &bCurBitmap);

	//HBITMAP bufferBMP = CreateCompatibleBitmap(hdc, bCurBitmap.bmWidth, bCurBitmap.bmHeight);
	//HGDIOBJ oldCompatBMP = SelectObject(hBufferDC, bufferBMP);

	SelectObject(hBufferDC, hbCurBitmap);
	RECT rControlSize = { 0, bCurBitmap.bmHeight / 6, bCurBitmap.bmWidth, bCurBitmap.bmHeight };
	HFONT cFont = CreateFont(16, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, APP_DEFFONT);
	SetTextColor(hBufferDC, APP_DEFTEXTCOLOR);
	SetBkMode(hBufferDC, TRANSPARENT);
	DrawTextW(hBufferDC, wsTitle.c_str(), -1, &rControlSize, DT_CENTER | DT_NOCLIP);

	StretchBlt(hdc, p.x, p.y, APP_DEFBTNW, APP_DEFBTNH, hBufferDC, 0, 0, bCurBitmap.bmWidth, bCurBitmap.bmHeight, SRCCOPY);

	DeleteObject(cFont);
	DeleteObject(hbCurBitmap);

	//RedrawWindow(c_hWnd, NULL, NULL, RDW_INVALIDATE);
	DeleteDC(hBufferDC);
	DeleteDC(hdc);

	//EndPaint(hWnd, &ps);

}

void DrawFrame(HWND hWnd) {

	PAINTSTRUCT ps;			//define paint structure
	HDC hdc = BeginPaint(hWnd, &ps);//handle device context
	while (true) {
	PaintMainWindow(hWnd, hdc);
	PaintMonitorData(hWnd, hdc, ps);
	PaintScreenTitle(hWnd, hdc, ps);
	PaintButton(hWnd, hdc, ps, IDC_BUTN01, L"Exit");
	PaintButton(hWnd, hdc, ps, IDC_BUTN02, L"Apply");
	PaintButton(hWnd, hdc, ps, IDC_BUTN03, L"Play RA2");
	EndPaint(hWnd, &ps);
	RedrawWindow(mainWnd, NULL, NULL, RDW_INVALIDATE);
	Sleep(1000 / 60);
	}
}