#pragma once

//static class for processing OWNERDRAW buttons and actions using them
class XCtl {

private:
	static int CtlX, CtlY, CtlW, CtlH;
	static const char ActLeave = 0, ActHover = 1, ActBtnDown = 2, ActBtnUp = 3;
	static int CtlID, CtlImage;
	static unsigned char CtlClass[APP_MAXCTL], CtlState[APP_MAXCTL];	//array that contains classes and states corresponding to control IDs
	static RECT CtlRect[APP_MAXCTL];									//control positions
	static std::wstring CtlText[APP_MAXCTL];							//array of strings
	static RECT txtRect;

	XCtl() {};

public:
	static const char CButton = 1, CChkBox = 2, CRadioBtn = 3;		//e.g. IDC_BUTN02 is IDC 12 and is a button, hence CtlTable[12] = 1
	static const char Unchecked = 0, Checked = 1;

private:
	//process methods
	static BOOL Process(HWND hWnd, int IDC, int OnAction);
	static BOOL Execute(const int &IDC);

public:
	//translation of actions to the main Process class method
	static BOOL OnLeave(HWND hWnd, int IDC);
	static BOOL OnHover(HWND hWnd, int IDC);
	static BOOL OnClick(HWND hWnd, int IDC, bool isBtnDown = FALSE, bool isDblClk = FALSE);

	//initial drawing of buttons
	static BOOL CtlCreate(int CtlType, int IDC, int x, int y, int w, int h, const std::wstring &Text = NULL);
	static BOOL CtlPaint(int IDC, HDC hdc = NULL, int IDB = BTN_DEFAULT);


	//control type array functions
	static BOOL GetCtlPos(const int &IDC, RECT &r);

	static BOOL SetCtlClass(int IDC, unsigned char Class);
	static int GetCtlClass(const int &IDC);

	static BOOL SetCtlState(int IDC, unsigned char State);
	static unsigned char GetCtlState(const int &IDC);
};