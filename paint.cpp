#include "main.h"
#include "XClasses.h"
#include "XCtl.h"

static int a = 0;
static bool increase = true;

static void PaintBackground() {

	HDC hBufferDC = CreateCompatibleDC(bDC);			//create compatible device context to use the bitmap in
	HPEN cPen = CreatePen(PS_SOLID, 1, RGB(120, 0, 0));
	HGDIOBJ oldPen = SelectObject(bDC, cPen);
	HGDIOBJ oldBrush = SelectObject(bDC, GetStockObject(NULL_BRUSH));	//make brush transparent
	HBITMAP bgHBMP = CreateCompatibleBitmap(bDC, clRect.right, clRect.bottom);
	bgHBMP = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDB_BITMAP1));	//background bitmap
	BITMAP bgBMP;								//create BITMAP from handle to bitmap
	GetObject(bgHBMP, sizeof(BITMAP), &bgBMP);	// ^^^^
	HGDIOBJ oldBMP = SelectObject(hBufferDC, bgHBMP);					//select bitmap to use with the compatible DC

	//stretch from DC2 to DC1 with offsets and new size (in this case fill the window)
	StretchBlt(bDC, 0, 0, clRect.right + 0, clRect.bottom + 0, hBufferDC, 0, 0, bgBMP.bmWidth, bgBMP.bmHeight, SRCCOPY);
	Rectangle(bDC, 0, 0, clRect.right, clRect.bottom);
	
	//draw logo 2
	HBITMAP logoBMP1 = CreateCompatibleBitmap(bDC, 340, 140);
	logoBMP1 = LoadBitmapW(mainInst, MAKEINTRESOURCEW(RA2LOGO));
	HGDIOBJ oldLogoBMP1 = SelectObject(hBufferDC, logoBMP1);
	TransparentBlt(bDC, 30, 80, 340, 140, hBufferDC, 0, 0, 340, 140, RGB(0, 0, 0));

	//draw logo 2
	HBITMAP logoBMP2 = CreateCompatibleBitmap(bDC, 360, 140);
	logoBMP2 = LoadBitmapW(mainInst, MAKEINTRESOURCEW(YRLOGO));
	HGDIOBJ oldLogoBMP2 = SelectObject(hBufferDC, logoBMP2);
	TransparentBlt(bDC, 395, 80, 360, 140, hBufferDC, 0, 0, 360, 140, RGB(0, 0, 0));

	SelectObject(bDC, oldPen);
	SelectObject(bDC, oldBrush);
	DeleteObject(oldPen);
	DeleteObject(oldBMP);
	DeleteObject(oldLogoBMP1);
	DeleteObject(oldLogoBMP2);
	DeleteObject(oldBrush);
	DeleteObject(bgHBMP);
	DeleteObject(logoBMP1);
	DeleteObject(logoBMP2);
	DeleteObject(oldLogoBMP1);
	DeleteObject(oldLogoBMP2);
	DeleteObject(cPen);
	DeleteDC(hBufferDC);
	ReleaseDC(mainWnd, bDC);

}

static void PaintMonitorData() {

	HDC hBufferDC = CreateCompatibleDC(bDC);

	HPEN cPen = CreatePen(PS_SOLID, 1, RGB(120, 0, 0));

	HFONT cFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Tahoma");
	RECT rect = { (clRect.right / 2) - 302, clRect.bottom - 15, clRect.right - 95, clRect.bottom };

	HBITMAP bHBMP = CreateCompatibleBitmap(bDC, rect.right, rect.bottom);
	
	HGDIOBJ oldBrush = SelectObject(hBufferDC, GetStockObject(NULL_BRUSH));
	SetBkMode(hBufferDC, TRANSPARENT);			//text setup
	SetTextColor(hBufferDC, RGB(160, 0, 0));
	
	HGDIOBJ oldBMP = SelectObject(hBufferDC, bHBMP);
	HGDIOBJ oldFont = SelectObject(hBufferDC, cFont);
	HGDIOBJ oldPen = SelectObject(hBufferDC, cPen);

	DrawTextW(hBufferDC, GetMonitorData().c_str(), -1, &rect, DT_CENTER | DT_NOCLIP);
	Rectangle(hBufferDC, rect.left, rect.top, rect.right, rect.bottom);

	BitBlt(bDC, rect.left, rect.top, rect.right, rect.bottom, hBufferDC, rect.left, rect.top, SRCPAINT);

	SelectObject(bDC, oldBrush);
	SelectObject(bDC, oldPen);
	SelectObject(bDC, oldFont);
	DeleteObject(cFont);
	DeleteObject(cPen);
	DeleteObject(oldBrush);
	DeleteObject(bHBMP);
	DeleteObject(oldBMP);
	DeleteObject(oldFont);
	DeleteObject(oldPen);
	DeleteDC(hBufferDC);
	ReleaseDC(mainWnd, bDC);
}

static void PaintScreenTitle(HWND hWnd, HDC hdc) {
	HDC hBufferDC = CreateCompatibleDC(bDC);
	HWND c_hWnd = GetDlgItem(hWnd, IDC_TEXT01);
	PAINTSTRUCT ps;
	BeginPaint(c_hWnd, &ps);

	HFONT cFont = CreateFont(16, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, APP_DEFFONT);

	POINT p = { 0 };
	MapWindowPoints(c_hWnd, GetParent(c_hWnd), &p, 1);
	RECT rect = { p.x, p.y, 605, 25 };

	SetBkMode(hdc, TRANSPARENT);			//text setup
	SetTextColor(hdc, RGB(255, 255, 0));

	HGDIOBJ oldFont = SelectObject(hdc, cFont);
	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

	DrawTextW(hdc, L"Red Alert 2 Configuration Tool", -1, &rect, DT_LEFT| DT_NOCLIP);

	SelectObject(hdc, oldFont);
	SelectObject(hdc, oldBrush);
	DeleteObject(cFont);
	DeleteDC(hBufferDC);
	ReleaseDC(hWnd, bDC);
	EndPaint(c_hWnd, &ps);

}

void DrawFrame() {

	bDC = CreateCompatibleDC(mainDC);
	HBITMAP backBMP = CreateCompatibleBitmap(mainDC, clRect.right, clRect.bottom);
	HGDIOBJ oldBMP = SelectObject(bDC, backBMP);
	ReleaseDC(mainWnd, mainDC);

	//init paint functions
	PaintBackground();
	PaintMonitorData();
	//PaintScreenTitle(hWnd, bDC);

	//paint controls
	/*CButtonProc::DoButtonPaint(IDC_BUTN01, bDC);
	CButtonProc::DoButtonPaint(IDC_BUTN02, bDC);
	CButtonProc::DoButtonPaint(IDC_BUTN03, bDC);
	CButtonProc::DoChkBoxPaint(IDC_BUTN04, bDC);
	CButtonProc::DoChkBoxPaint(IDC_BUTN05, bDC);
	*/

	XCtl::CtlPaint(IDC_APPLY, bDC);
	XCtl::CtlPaint(IDC_BROWSE, bDC);
	XCtl::CtlPaint(IDC_EXIT, bDC);

	//RedrawWindow(mainWnd, NULL, NULL, RDW_INVALIDATE);
	//Sleep(1000 / 60);

	//SelectObject(bDC, oldBMP);

	DeleteObject(oldBMP);
	DeleteObject(backBMP);
	//DeleteDC(bDC);

}