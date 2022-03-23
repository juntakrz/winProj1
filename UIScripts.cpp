#include "main.h"
#include "XClasses.h"
#include "XCtl.h"

static void CreateUIElement(HWND hWnd, int IDC, LPCWSTR UIText, int posX, int posY, int sizeW, int sizeH, int ParamA, int ParamB) {

	if (IDC >= 11 && IDC <= 30) {					// create BUTTON

		LONG StyleList = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		if (!sizeW) sizeW = APP_DEFBTNW;
		if (!sizeH) sizeH = APP_DEFBTNH;
		
		//class, text, style, x, y, w, h, parent window, ID, unused, unused
		CreateWindow(WC_BUTTON, UIText, StyleList, posX, posY, sizeW, sizeH, hWnd, (HMENU)IDC, NULL, NULL);

		//change button style depending on ParamA
		if (ParamA == 1) {
			SendDlgItemMessage(hWnd, IDC, BM_SETSTYLE, BS_CHECKBOX, NULL);
			CButtonProc::SetCtlClass(IDC, CButtonProc::CChkBox);
		}
		else if (ParamA == 2) {
			SendDlgItemMessage(hWnd, IDC, BM_SETSTYLE, BS_RADIOBUTTON, NULL);
			CButtonProc::SetCtlClass(IDC, CButtonProc::CRadioBtn);
		}
		else CButtonProc::SetCtlClass(IDC, CButtonProc::CButton);		//add control class to table

		//check if button should be OwnerDraw
		if (ParamB == 1) SendDlgItemMessage(hWnd, IDC, BM_SETSTYLE, BS_OWNERDRAW, NULL);
	}
	else if (IDC >= 31 && IDC <= 50) {				// create STATIC (text)

		LONG StyleList = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		
		if (ParamA == 1) StyleList |= WS_BORDER;
		else if (ParamA == 2) StyleList |= SS_CENTER | WS_BORDER;
		else if (ParamA == 3) StyleList |= SS_CENTER;

		CreateWindow(WC_STATIC, UIText, StyleList, posX, posY, sizeW, sizeH, hWnd, (HMENU)IDC, NULL, NULL);

		HDC hdc = GetDC(GetDlgItem(hWnd, IDC));
		SetBkMode(hdc, TRANSPARENT);
		ReleaseDC(hWnd, hdc);
		/*if (ParamA == 1) {						//add border

			HWND hWndTemp = GetDlgItem(hWnd, IDC);																	//get a handle to current ID
			LONG styleTemp = GetWindowLong(hWndTemp, GWL_STYLE) | WS_BORDER;										//add border to style
			SetWindowLong(hWndTemp, GWL_STYLE, styleTemp);															//refresh style
			SetWindowPos(hWndTemp, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);	//required to change STATIC style

		}*/

	}
	else if (IDC >= 51 && IDC <= 70) {				// create EDIT (input)

		LONG StyleList = WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL;

		if (ParamB == 1) StyleList |= ES_PASSWORD;
		else if (ParamB == 2) StyleList |= ES_NUMBER;

		CreateWindow(WC_EDIT, UIText, StyleList, posX, posY, sizeW, sizeH, hWnd, (HMENU)IDC, NULL, NULL);

		//set input limit if ParamA is defined
		if (ParamA > 0) {
			SendDlgItemMessage(hWnd, IDC, EM_SETLIMITTEXT, ParamA, NULL);
		}
	}

	else if (IDC >= 71 && IDC <= 90) {				//create COMBOBOX

		LONG StyleList = WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL;

		if (ParamA == 1) StyleList &= ~CBS_DROPDOWNLIST | CBS_DROPDOWN;					//editable
		else if (ParamA == 2) StyleList |= CBS_SORT;										//basic sorted
		else if (ParamA == 3) StyleList &= ~CBS_DROPDOWNLIST | CBS_DROPDOWN | CBS_SORT;	//editable & basic sorted

		CreateWindow(WC_COMBOBOX, UIText, StyleList, posX, posY, sizeW, sizeH, hWnd, (HMENU)IDC, NULL, NULL);

	}
	else if (IDC >= 91 && IDC <= 110) {				//create LISTBOX

		LONG StyleList = WS_VISIBLE | WS_CHILD | WS_BORDER;

		CreateWindow(WC_LISTBOX, UIText, StyleList, posX, posY, sizeW, sizeH, hWnd, (HMENU)IDC, NULL, NULL);

	}
	else if (IDC >= 111 && IDC <= 130) {			//create GROUPBOX
	
		LONG StyleList = WS_VISIBLE | WS_CHILD | BS_GROUPBOX;

		CreateWindow(WC_BUTTON, UIText, StyleList, posX, posY, sizeW, sizeH, hWnd, (HMENU)IDC, NULL, NULL);
	
	}

}

void PopulateComboBox( _In_ HWND hWnd, int Option, int ParamA)
{
	//clear the combobox
	SendDlgItemMessage(hWnd, IDC_COMB01, CB_RESETCONTENT, NULL, NULL);

	switch (Option) {

	case IDC_BUTN04 :	//List all resolutions available on a driver level
	{
		//getting the size of the resArray by diving resArray by the size of its element
		//int resArraySize = sizeof(resArray) / sizeof(resArray[0]);

		//getting the size of the resArray: a memory address of the array right after the end of the array with total size of resArray subtracted from it
		int resArraySize = *(&iResArray + 1) - iResArray;		
		wchar_t wRes[16];

		for (int posX = 0; posX < resArraySize; ++posX) {
			if (iResArray[posX][0] && iResArray[posX][1]) {						//only add to combobox if array position has resolution data
				wsprintfW(wRes, L"%d x %d", iResArray[posX][0], iResArray[posX][1]);
				SendDlgItemMessage(hWnd, IDC_COMB01, CB_ADDSTRING, NULL, (LPARAM)wRes);
			}
		}
		SendDlgItemMessage(hWnd, IDC_COMB01, CB_SETMINVISIBLE, 12, 0);			//number of items to show in dropdown list
		break;
	}

	case IDC_BUTN05:	//set desktop resolution
	{
		wchar_t wRes[16];
		int resW, resH;
		GetDesktopResolution(resW, resH);
		wsprintfW(wRes, L"%d x %d", resW, resH);

		SendDlgItemMessage(hWnd, IDC_COMB01, CB_SETMINVISIBLE, 0, 0);
		SendDlgItemMessage(hWnd, IDC_COMB01, CB_ADDSTRING, NULL, (LPARAM)wRes);
		break;
	}

	}

	//set combobox selection based on position received in ParamA
	SendDlgItemMessage(hWnd, IDC_COMB01, CB_SETCURSEL, ParamA, NULL);

}

void CreateUIMain(HWND hWnd) {

	//BUTTON
	//create buttons

	//create checkboxes for resolutions
	CreateUIElement(hWnd, IDC_BUTN04, L"Supported", 170, clRect.bottom - 62 - APP_DEFBTNH * 2, 18, 18, 2, 1);
	CreateUIElement(hWnd, IDC_BUTN05, L"Desktop", 170, clRect.bottom - 55 - APP_DEFBTNH, 18, 18, 2, 1);

	//create checkboxes for movies
	//CreateUIElement(hWnd, IDC_BUTN07, L"Skip intro", 30, 220, 80, 25, 1);
	//CreateUIElement(hWnd, IDC_BUTN08, L"Stretch", 170, 220, 80, 25, 1);
	//CreateUIElement(hWnd, IDC_BUTN09, L"Disable AI MP Money Cheats", 30, 298, 200, 32, 1);

	//check checkboxes
	CButtonProc::SetCtlState(IDC_BUTN04, CButtonProc::Checked);
	CButtonProc::SetCtlState(IDC_BUTN05, CButtonProc::Unchecked);
	CheckDlgButton(hWnd, IDC_BUTN07, iINIParam[1]);
	CheckDlgButton(hWnd, IDC_BUTN08, iINIParam[2]);

	//COMBOBOX
	//create static combobox list
	CreateUIElement(hWnd, IDC_COMB01, NULL, 20, clRect.bottom - 64 - APP_DEFBTNH * 2, 130, 240);

	//initialize default combo box which is tied to IDC_BUTN04
	PopulateComboBox(hWnd, IDC_BUTN04, iINIParam[0]);

	XCtl::CtlCreate(CC_BUTTON, IDC_APPLY, clRect.right - 20 - APP_DEFBTNW, clRect.bottom - 69 - APP_DEFBTNH * 3, APP_DEFBTNW, APP_DEFBTNH, L"Apply");
	XCtl::CtlCreate(CC_BUTTON, IDC_BROWSE, clRect.right - 20 - APP_DEFBTNW, clRect.bottom - 62 - APP_DEFBTNH * 2, APP_DEFBTNW, APP_DEFBTNH, L"Browse");
	XCtl::CtlCreate(CC_BUTTON, IDC_EXIT, clRect.right - 20 - APP_DEFBTNW, clRect.bottom - 55 - APP_DEFBTNH, APP_DEFBTNW, APP_DEFBTNH, L"Exit");
	
	//SendDlgItemMessage(hWnd, IDC_TEXT01, WM_SETFONT, (WPARAM)boldFont, NULL);	//bold parameter for the title text
	//SendDlgItemMessage(hWnd, IDC_TEXT02, WM_SETFONT, (WPARAM)smallFont, NULL);

}