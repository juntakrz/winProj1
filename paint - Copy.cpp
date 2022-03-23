#include "main.h"

HDC mainDC;
HDC bDC;

void PaintMainWindow(HWND hWnd) {

	PAINTSTRUCT ps;			//define paint structure
	HDC hdc = BeginPaint(hWnd, &ps);				//handle device context

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

	EndPaint(hWnd, &ps);

}

void PaintMonitorData(HWND hWnd) {

	HWND c_hWnd = GetDlgItem(hWnd, IDC_TEXT02);
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(c_hWnd, &ps);

	HPEN cPen = CreatePen(PS_SOLID, 1, RGB(120, 0, 0));

	HFONT cFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Tahoma");
	RECT rect = { 0,0,605,25 };

	SetBkMode(hdc, TRANSPARENT);			//text setup
	SetTextColor(hdc, RGB(160, 0, 0));
	
	HGDIOBJ oldFont = SelectObject(hdc, cFont);
	HGDIOBJ oldPen = SelectObject(hdc, cPen);
	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

	DrawTextW(hdc, GetMonitorData().c_str(), -1, &rect, DT_CENTER | DT_NOCLIP);
	Rectangle(hdc, 0, 0, 605, 25);

	SelectObject(hdc, oldFont);
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(cFont);
	DeleteObject(cPen);
	EndPaint(c_hWnd, &ps);
}

void PaintScreenTitle(HWND hWnd) {

	HWND c_hWnd = GetDlgItem(hWnd, IDC_TEXT01);
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(c_hWnd, &ps);

	//HPEN cPen = CreatePen(PS_SOLID, 1, RGB(120, 0, 0));

	HFONT cFont = CreateFont(16, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, APP_DEFFONT);
	RECT rect = { 0,0,605,25 };

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

void PaintButton(HWND hWnd, DWORD IDC, std::wstring wsTitle = {}) {
	bool isDrawing = true;
	HWND c_hWnd = GetDlgItem(hWnd, IDC);
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(c_hWnd, &ps);
	HDC hBufferDC = CreateCompatibleDC(hdc);

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

	StretchBlt(hdc, 0, 0, APP_DEFBTNW, APP_DEFBTNH, hBufferDC, 0, 0, bCurBitmap.bmWidth, bCurBitmap.bmHeight, SRCCOPY);

	DeleteObject(cFont);
	DeleteObject(hbCurBitmap);

	//RedrawWindow(c_hWnd, NULL, NULL, RDW_INVALIDATE);
	DeleteDC(hBufferDC);
	DeleteDC(hdc);

	EndPaint(hWnd, &ps);

}

void DrawFrame(HWND hWnd) {

	PaintMainWindow(hWnd);
	//PaintMonitorData(hWnd);
	//PaintScreenTitle(hWnd);
	PaintButton(hWnd, IDC_BUTN01, L"Exit");
	PaintButton(hWnd, IDC_BUTN02, L"Apply");
	PaintButton(hWnd, IDC_BUTN03, L"Play RA2");
}