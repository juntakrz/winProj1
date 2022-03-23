#pragma once

//define button action bitmaps
#ifndef CTL_BITMAPS
#define BTN_DEFAULT IDB_BITMAP2
#define BTN_HOVER IDB_BITMAP3
#define BTN_CLICK IDB_BITMAP4
#define BTN_CHK IDB_BITMAP7
#define BTN_UCHK IDB_BITMAP8
#endif

//static class for processing OWNERDRAW buttons and actions using them
class CButtonProc {

private:
	static int CtlX, CtlY, CtlW, CtlH;
	static const char ActLeave = 0, ActHover = 1, ActBtnDown = 2, ActBtnUp = 3;
	static int CtlID, CtlImage;
	static unsigned char CtlTable[200], CtlState[200];				//array that contains classes and states corresponding to control IDs
	static RECT txtRect;

	CButtonProc() {};

public:
	static const char CButton = 1, CChkBox = 2, CRadioBtn = 3;		//e.g. IDC_BUTN02 is IDC 12 and is a button, hence CtlTable[12] = 1
	static const char Unchecked = 0, Checked = 1;

private:
	//process methods
	static BOOL Process(HWND hWnd, int IDC, int OnAction);
	static BOOL Execute(const int IDC);

public:
	//translation of actions to the main Process class method
	static BOOL OnLeave(HWND hWnd, int IDC);
	static BOOL OnHover(HWND hWnd, int IDC);
	static BOOL OnClick(HWND hWnd, int IDC, bool isBtnDown = FALSE, bool isDblClk = FALSE);

	//initial drawing of buttons
	static BOOL DoButtonPaint(int IDC, HDC hdc = NULL, int IDB = BTN_DEFAULT);
	static BOOL DoChkBoxPaint(int IDC, HDC hdc = NULL);

	//control type array functions
	static BOOL SetCtlClass(int IDC, unsigned char Class);
	static int GetCtlClass(int IDC);

	static BOOL SetCtlState(int IDC, unsigned char State);
	static unsigned char GetCtlState(int IDC);

	//internal dimensions
	static BOOL SetDimensions(int x, int y, int w, int h);
	static BOOL GetDimensions(int& x, int& y, int& w, int& h);
};