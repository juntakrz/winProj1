#include "main.h"
#include "XCtl.h"

//very local variables
static int iCurAction = 0;
static bool bDblClk = FALSE;

//initialize class variables
int XCtl::CtlX = 0, XCtl::CtlY = 0, XCtl::CtlW = APP_DEFBTNW, XCtl::CtlH = APP_DEFBTNH;
int XCtl::CtlImage = NULL, XCtl::CtlID = NULL;
RECT XCtl::txtRect = { CtlX, CtlH / 6, CtlW, CtlH };
unsigned char XCtl::CtlClass[APP_MAXCTL] = { 0 }, XCtl::CtlState[APP_MAXCTL] = { 0 };
RECT XCtl::CtlRect[APP_MAXCTL] = { 0 };					//array storing control zones
std::wstring XCtl::CtlText[APP_MAXCTL];

//Private methods

BOOL XCtl::Process(HWND hWnd, int IDC, int OnAction) {

	//get controller ID and execute a corresponding action
	XCtl::CtlID = IDC;
	iCurAction = OnAction;

	//class CButton actions
	if (CtlClass[CtlID] == CButton) {
		if (OnAction == XCtl::ActLeave) XCtl::CtlImage = BTN_DEFAULT;
		else if (OnAction == XCtl::ActHover) XCtl::CtlImage = BTN_HOVER;
		else if (OnAction == XCtl::ActBtnDown) XCtl::CtlImage = BTN_CLICK;
		else if (OnAction == XCtl::ActBtnUp) {
			XCtl::CtlImage = BTN_HOVER;
			if (!bDblClk) {
				//flash button
				for (int i = 0; i < 2; i++) {
					XCtl::CtlPaint(IDC, NULL, BTN_HOVER);
					Sleep(77);
					XCtl::CtlPaint(IDC, NULL, BTN_CLICK);
					Sleep(77);
				}
				XCtl::CtlPaint(IDC, NULL, BTN_DEFAULT);
				Execute(XCtl::CtlID);
				return true;
			}
			else if (bDblClk) {
				bDblClk = FALSE;
			}
		}

		//repaint button based on its current state
		XCtl::CtlPaint(IDC, NULL, CtlImage);

		return true;
	}
	else if (CtlClass[CtlID] == CRadioBtn) {
		if (OnAction == XCtl::ActBtnUp) {

			Execute(XCtl::CtlID);
			return true;
		}
	}

	//some error / unable to process
	return false;
}

//Method to execute button actions
BOOL XCtl::Execute(const int &IDC) {

	//code for button actions
	switch (IDC) {

	default:
		break;

	}
	return true;
}

//Public methods

BOOL XCtl::OnLeave(HWND hWnd, int IDC) { Process(hWnd, IDC, XCtl::ActLeave); return true; }
BOOL XCtl::OnHover(HWND hWnd, int IDC) { Process(hWnd, IDC, XCtl::ActHover); return true; }
BOOL XCtl::OnClick(HWND hWnd, int IDC, bool isBtnDown, bool isDblClk)
{
	bDblClk = isDblClk;
	//else if (!isDblClk) CButtonProc::bDblClk = false;
	if (isBtnDown) {
		Process(hWnd, IDC, XCtl::ActBtnDown);
		return true;
	}
	else
	{
		Process(hWnd, IDC, XCtl::ActBtnUp);
		return true;
	}
}

BOOL XCtl::CtlCreate(int CtlType, int IDC, int x, int y, int w, int h, const std::wstring &Text) {

	//create virtual control
	
	//add virtual control position and dimensions to control table as well as type
	XCtl::CtlRect[IDC] = { x, y, x + w, y + h };
	XCtl::SetCtlClass(IDC, CtlType);
	XCtl::CtlText[IDC] = Text;
	return true;
}

BOOL XCtl::CtlPaint(int IDC, HDC hdc, int IDB) {

	//make color differ and text slightly displaced when clicking the button
	COLORREF crColor;
	if (iCurAction == ActBtnDown) {
		crColor = RGB(255, 200, 0);
		XCtl::txtRect = { CtlX + 2, (CtlH / 6) + 1, CtlW, CtlH };
	}
	else {
		crColor = APP_DEFTEXTCOLOR;
		XCtl::txtRect = { CtlX, CtlH / 6, CtlW, CtlH };
	}

	if (XCtl::CtlState[IDC] == 1) IDB = BTN_HOVER;
	else if (!XCtl::CtlState[IDC]) IDB = BTN_DEFAULT;

	HWND hWnd = mainWnd;
	if (!hdc) hdc = GetDC(mainWnd);

	//paint code
	HDC hBufferDC = CreateCompatibleDC(hdc);
	int TextStyleList = DT_CENTER | DT_NOCLIP;

	//extract correct control dimensions from the control zone array
	RECT CtlRect = { XCtl::CtlRect[IDC].left, XCtl::CtlRect[IDC].top, XCtl::CtlRect[IDC].right - XCtl::CtlRect[IDC].left, XCtl::CtlRect[IDC].bottom - XCtl::CtlRect[IDC].top };

	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	HBITMAP hbCurBitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDB));
	HGDIOBJ oldBMP = SelectObject(hBufferDC, hbCurBitmap);
	HGDIOBJ oldFont = SelectObject(hBufferDC, GetStockObject(SYSTEM_FONT));
	SetBkMode(hBufferDC, TRANSPARENT);
	SetTextColor(hBufferDC, crColor);
	DrawTextW(hBufferDC, XCtl::CtlText[IDC].c_str(), -1, &XCtl::txtRect, TextStyleList);

	BitBlt(hdc, CtlRect.left, CtlRect.top, CtlRect.right, CtlRect.bottom, hBufferDC, 0, 0, SRCCOPY);
	
	SelectObject(hBufferDC, oldBMP);
	SelectObject(hBufferDC, oldFont);
	DeleteObject(oldBMP);
	DeleteObject(oldFont);
	DeleteObject(hbCurBitmap);
	DeleteDC(hBufferDC);
	ReleaseDC(hWnd, hdc);
	EndPaint(hWnd, &ps);

	return true;
}

BOOL XCtl::GetCtlPos(const int &IDC, RECT& r) {

	r = XCtl::CtlRect[IDC];
	return true;

}

BOOL XCtl::SetCtlClass(int IDC, unsigned char CtlClass) {
	
	if (CtlClass > -1 && CtlClass < 10) {
		XCtl::CtlClass[IDC] = CtlClass;
		return true;
	}
	else return false;

}

int XCtl::GetCtlClass(const int &IDC) {

	if (IDC > -1 && IDC < 200) return XCtl::CtlClass[IDC];
	else return 0;		//0 means no control class was defined at this position / some kind of error

}

BOOL XCtl::SetCtlState(int IDC, unsigned char State) {

	if (IDC > -1 && IDC < 200) {
		XCtl::CtlState[IDC] = State;
		XCtl::CtlPaint(IDC);
		return true;
	}
	else return false;
}
unsigned char XCtl::GetCtlState(const int &IDC) {

	if (IDC > -1 && IDC < 200) {
		return XCtl::CtlState[IDC];
	}
	else return false;					//will always return 0 if something is wrong

}