#include "main.h"
#include "XClasses.h"

//very local variables
static int iCurAction = 0;
static bool bDblClk = FALSE;

//initialize class variables
int CButtonProc::CtlX = 0, CButtonProc::CtlY = 0, CButtonProc::CtlW = APP_DEFBTNW, CButtonProc::CtlH = APP_DEFBTNH;
int CButtonProc::CtlImage = NULL, CButtonProc::CtlID = NULL;
unsigned char CButtonProc::CtlTable[200] = { 0 }, CButtonProc::CtlState[200] = { 0 };
RECT CButtonProc::txtRect = { CtlX, CtlH / 6, CtlW, CtlH };

//Private methods

BOOL CButtonProc::Process(HWND hWnd, int IDC, int OnAction) {

	//get controller ID and execute a corresponding action
	CButtonProc::CtlID = IDC;
	iCurAction = OnAction;

	//class CButton actions
	if (CtlTable[CtlID] == CButton) {
		if (OnAction == CButtonProc::ActLeave) CButtonProc::CtlImage = BTN_DEFAULT;
		else if (OnAction == CButtonProc::ActHover) CButtonProc::CtlImage = BTN_HOVER;
		else if (OnAction == CButtonProc::ActBtnDown) CButtonProc::CtlImage = BTN_CLICK;
		else if (OnAction == CButtonProc::ActBtnUp) {
			CButtonProc::CtlImage = BTN_HOVER;
			if (!bDblClk) {
				//flash button
				for (int i = 0; i < 2; i++) {
					DoButtonPaint(IDC, NULL, BTN_HOVER);
					Sleep(77);
					DoButtonPaint(IDC, NULL, BTN_CLICK);
					Sleep(77);
				}
				DoButtonPaint(IDC, NULL, BTN_DEFAULT);
				Execute(CButtonProc::CtlID);
				return true;
			}
			else if (bDblClk) {
				bDblClk = FALSE;
			}
		}

		//repaint button based on its current state
		DoButtonPaint(IDC, NULL, CtlImage);

		return true;
	}
	else if (CtlTable[CtlID] == CRadioBtn) {
		if (OnAction == CButtonProc::ActBtnUp) {

			Execute(CButtonProc::CtlID);
			return true;
		}
	}

	//some error / unable to process
	return false;
}

//Method to execute button actions
BOOL CButtonProc::Execute(const int IDC) {

	//code for button actions
	switch (IDC) {

	case IDC_BUTN01: {				//Exit button
		
		PostQuitMessage(0);
		//DestroyWindow(mainWnd);
		break;
	}

	case IDC_BUTN02: {				//Browse button

		//LaunchApp(APPPATH1);
		//PostQuitMessage(0);
		::ShellExecuteW(NULL, NULL, L".\\", NULL, NULL, SW_SHOWNORMAL);
		break;

	}

	case IDC_BUTN03: {				//Apply

		SetINIParameters(INIFILE1);
		::OutputDebugStringW(L"CButtonProc.Execute::IDC_BUTN03: INI file written.\n");
		break;

	}

	case IDC_BUTN04: {

		if (!IsDlgButtonChecked(mainWnd, IDC_BUTN04)) {
			CButtonProc::SetCtlState(IDC_BUTN04, CButtonProc::Checked);
			CButtonProc::SetCtlState(IDC_BUTN05, CButtonProc::Unchecked);
			PopulateComboBox(mainWnd, IDC_BUTN04, iINIParam[0]);
			EnableWindow(GetDlgItem(mainWnd, IDC_COMB01), TRUE);
			DoChkBoxPaint(IDC_BUTN04);
			DoChkBoxPaint(IDC_BUTN05);
		}
		break;
	}

	case IDC_BUTN05: {

		if (!IsDlgButtonChecked(mainWnd, IDC_BUTN05)) {
			CButtonProc::SetCtlState(IDC_BUTN04, CButtonProc::Unchecked);
			CButtonProc::SetCtlState(IDC_BUTN05, CButtonProc::Checked);
			PopulateComboBox(mainWnd, IDC_BUTN05);
			EnableWindow(GetDlgItem(mainWnd, IDC_COMB01), FALSE);
			DoChkBoxPaint(IDC_BUTN04);
			DoChkBoxPaint(IDC_BUTN05);
		}
		break;
	}

	}
	return true;
}

//Public methods

BOOL CButtonProc::OnLeave(HWND hWnd, int IDC) { Process(hWnd, IDC, CButtonProc::ActLeave); return true; }
BOOL CButtonProc::OnHover(HWND hWnd, int IDC) { Process(hWnd, IDC, CButtonProc::ActHover); return true; }
BOOL CButtonProc::OnClick(HWND hWnd, int IDC, bool isBtnDown, bool isDblClk)
{
	bDblClk = isDblClk;
	//else if (!isDblClk) CButtonProc::bDblClk = false;
	if (isBtnDown) {
		Process(hWnd, IDC, CButtonProc::ActBtnDown);
		return true;
	}
	else
	{
		Process(hWnd, IDC, CButtonProc::ActBtnUp);
		return true;
	}
}

BOOL CButtonProc::DoButtonPaint(int IDC, HDC hdc, int IDB) {

	//make color differ and text slightly displaced when clicking the button
	COLORREF crColor;
	if (iCurAction == ActBtnDown) {
		crColor = RGB(255, 200, 0);
		CButtonProc::txtRect = { CtlX + 2, (CtlH / 6) + 1, CtlW, CtlH };
	}
	else {
		crColor = APP_DEFTEXTCOLOR;
		CButtonProc::txtRect = { CtlX, CtlH / 6, CtlW, CtlH };
	}

	//set contexts and handles
	HWND hWnd = GetDlgItem(mainWnd, IDC);		//determine handle to window for current control
	if (!hdc) hdc = GetDC(mainWnd);
	wchar_t strText[32];

	//paint code
	HDC hBufferDC = CreateCompatibleDC(hdc);
	int TextStyleList = DT_CENTER | DT_NOCLIP;

	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	POINT p = { 0 };
	MapWindowPoints(hWnd, GetParent(hWnd), &p, 1);		//get position of a window
	HBITMAP hbCurBitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDB));
	HGDIOBJ oldBMP = SelectObject(hBufferDC, hbCurBitmap);
	HGDIOBJ oldFont = SelectObject(hBufferDC, GetStockObject(SYSTEM_FONT));
	SetBkMode(hBufferDC, TRANSPARENT);
	SetTextColor(hBufferDC, crColor);
	if (!GetDlgItemTextW(hWnd, IDC, strText, sizeof(strText)))		//if we are getting an empty text - current HWND is that of a child control and thus has no children with IDC
		GetDlgItemTextW(mainWnd, IDC, strText, sizeof(strText));	//so we step up to the parent and retrieve the text from IDC related to it
	DrawTextW(hBufferDC, strText, -1, &CButtonProc::txtRect, TextStyleList);
	StretchBlt(hdc, p.x, p.y, CButtonProc::CtlW, CButtonProc::CtlH, hBufferDC, CButtonProc::CtlX, CButtonProc::CtlY, CButtonProc::CtlW, CButtonProc::CtlH, SRCCOPY);
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

BOOL CButtonProc::DoChkBoxPaint(int IDC, HDC hdc) {

	static int iBtnState;
	if (CButtonProc::CtlState[IDC]) iBtnState = BTN_CHK;
	else iBtnState = BTN_UCHK;

	//set contexts and handles
	HWND hWnd = GetDlgItem(mainWnd, IDC);		//determine handle to window for current control
	if (!hdc) hdc = GetDC(mainWnd);
	
	//paint code
	HDC hBufferDC = CreateCompatibleDC(hdc);
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	POINT p = { 0 };
	MapWindowPoints(hWnd, GetParent(hWnd), &p, 1);
	HBITMAP hbCurBitmap = CreateCompatibleBitmap(hdc, CButtonProc::CtlW, CButtonProc::CtlH);
	hbCurBitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCEW(iBtnState));
	HGDIOBJ oldBMP = SelectObject(hBufferDC, hbCurBitmap);
	StretchBlt(hdc, p.x, p.y, CButtonProc::CtlW, CButtonProc::CtlH, hBufferDC, CButtonProc::CtlX, CButtonProc::CtlY, CButtonProc::CtlW, CButtonProc::CtlH, SRCCOPY);
	SelectObject(hBufferDC, oldBMP);
	DeleteObject(oldBMP);
	DeleteObject(hbCurBitmap);
	DeleteDC(hBufferDC);
	ReleaseDC(hWnd, hdc);
	EndPaint(hWnd, &ps);

	return true;
}

BOOL CButtonProc::SetCtlClass(int IDC, unsigned char Class) {
	
	if (Class > -1 && Class < 10) {
		CButtonProc::CtlTable[IDC] = Class;
		return true;
	}
	else return false;

}

int CButtonProc::GetCtlClass(int IDC) {

	if (IDC > -1 && IDC < 200) return CButtonProc::CtlTable[IDC];
	else return 0;		//0 means no control class was defined at this position / some kind of error

}

BOOL CButtonProc::SetCtlState(int IDC, unsigned char State) {

	if (IDC > -1 && IDC < 200) {
		CButtonProc::CtlState[IDC] = State;
		return true;
	}
	else return false;
}
unsigned char CButtonProc::GetCtlState(int IDC) {

	if (IDC > -1 && IDC < 200) {
		return CButtonProc::CtlState[IDC];
	}
	else return false;					//will always return 0 if something is wrong

}

BOOL CButtonProc::SetDimensions(int x, int y, int w, int h) {
	CButtonProc::CtlX = x;
	CButtonProc::CtlY = y;
	CButtonProc::CtlW = w;
	CButtonProc::CtlH = h;
	CButtonProc::txtRect = { CButtonProc::CtlX, CButtonProc::CtlH / 6, CButtonProc::CtlW, CButtonProc::CtlH };
	return true;
}

BOOL CButtonProc::GetDimensions(int& x, int& y, int& w, int& h) {
	x = CButtonProc::CtlX;
	y = CButtonProc::CtlY;
	w = CButtonProc::CtlW;
	h = CButtonProc::CtlH;
	return true;
}