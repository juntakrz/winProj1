#include "main.h"
#include "Threads.h"

//global app variables
HWND mainWnd;			//handle to main window
HINSTANCE mainInst;		//handle to main instance
HDC mainDC, bDC;		//main DC and buffer DC
RECT clRect;			//usable main window area
BOOL bChangesMade = FALSE;		//variable for any unsaved changes

//window procedure
LRESULT CALLBACK mainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)			//handle received window message
	{
	case WM_CREATE: {

		//preparing data
		GetClientRect(hWnd, &clRect);		//get useful window area
		GPUResolutionsToArray();			//gather resolutions supported by display and GPU
		GetINIParameters(INIFILE1);			//read parameters from RA2 INI

		//creating main GUI
		CreateUIMain(hWnd);
	
		HWND hToolTip1 = CreateWindow(TOOLTIPS_CLASSW, 0, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, GetDlgItem(mainWnd, IDC_BUTN02), 0, GetModuleHandle(NULL), NULL);
		//SetWindowPos(hToolTip1, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

		TOOLINFO ti = {};
		ti.cbSize = sizeof(TOOLINFO);
		ti.uFlags = TTF_SUBCLASS;
		ti.hwnd = GetDlgItem(hWnd, IDC_BUTN02);
		ti.hinst = GetModuleHandle(NULL);
		ti.uId = (UINT_PTR)hWnd;
		ti.lpszText = (LPWSTR)L"DEBUG: Test tooltip"; \
			GetClientRect(GetDlgItem(hWnd, IDC_BUTN02), &ti.rect);
		SendMessage(hToolTip1, TTM_ADDTOOL, 0, (LPARAM)&ti);

		break;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}

	case WM_COMMAND: {
		//different ways to address button events - either by the HWND name or by ID
		//if ((HIWORD(wParam) == BN_CLICKED) and ((HWND)lParam) == hBtn1) {
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDC_BUTN01) {
			//if (LOWORD(wParam) == IDC_BUTN01) {

			//DestroyWindow(hWnd);

		};

		//handle movie checkboxes
		if ((LOWORD(wParam) == IDC_BUTN07) && !IsDlgButtonChecked(hWnd, IDC_BUTN07)) {
			CheckDlgButton(hWnd, IDC_BUTN07, TRUE);
		}
		else if ((LOWORD(wParam) == IDC_BUTN07) && IsDlgButtonChecked(hWnd, IDC_BUTN07)) {
			CheckDlgButton(hWnd, IDC_BUTN07, FALSE);
		};

		if ((LOWORD(wParam) == IDC_BUTN08) && !IsDlgButtonChecked(hWnd, IDC_BUTN08)) {
			CheckDlgButton(hWnd, IDC_BUTN08, TRUE);
		}
		else if ((LOWORD(wParam) == IDC_BUTN08) && IsDlgButtonChecked(hWnd, IDC_BUTN08)) {
			CheckDlgButton(hWnd, IDC_BUTN08, FALSE);
		};

		if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == IDC_COMB01) {

			//write combobox selection to resolution ID parameter in INIParam array
			iINIParam[0] = SendDlgItemMessage(hWnd, IDC_COMB01, CB_GETCURSEL, NULL, NULL);

		}

		break;
	}

	case WM_NOTIFY:	{

		break;

	}

	case WM_DRAWITEM: {

		break;
	}

	case WM_PAINT: {

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		//execute paint scripts that create the back buffer bitmap and then blit it to the main device context
		DrawFrame();
		BitBlt(hdc, 0, 0, clRect.right, clRect.bottom, bDC, 0, 0, SRCCOPY);
		//cleanup
		EndPaint(hWnd, &ps);
		DeleteDC(hdc);
		break;
	}

	case WM_CTLCOLORBTN: {
		break;
	}

	case WM_TIMER: {
		//DrawFrame(hWnd);
		//RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		break;
	}

	}

	//if some message isn't processed above - handle it off to Windows to do do its default things with it
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
	//check if app is in RA2 directory
	FILE* file;
	if (fopen_s(&file, ".\\RA2.INI", "r")) {
		MessageBox(nullptr, L"Red Alert 2 was not detected!", L"ERROR", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
	}
	fclose(file);

	//create icon for the app
	HICON hIcon1 = static_cast<HICON>(::LoadImage(hInstance,
		MAKEINTRESOURCEW(IDI_ICON1),			//icon name in resource.rc
		IMAGE_ICON,
		::GetSystemMetrics(SM_CXICON),			//get appropraite icon size
		::GetSystemMetrics(SM_CYICON),
		LR_DEFAULTCOLOR));

	//declare global handle to main instance
	mainInst = hInstance;

	//register window class
	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = mainWndProc;								//window procedure pointer
	wc.hInstance = mainInst;									//handle to the app instance
	wc.lpszClassName = APP_WINCLASS;							//string identifying window class
	wc.hIcon = hIcon1;											//custom icon
	wc.hIconSm = hIcon1;
	wc.hCursor = NULL;											//use default cursor
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);			//window background
	wc.lpszMenuName = NULL;										//no custom window menu

	RegisterClassEx(&wc);

	//calculate initial mainWnd position / center it on the screen / adjust client area to be equal to app resolution
	static RECT rWinSize = InitWinCoordinates(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, TRUE);

	//create the window
	mainWnd = CreateWindowExW(
		0,									//optional window style
		APP_WINCLASS,						//window class
		APP_WINNAME,						//window name
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,				//window style

		//position (x,y) and size (w, h)
		rWinSize.left, rWinSize.top, rWinSize.right, rWinSize.bottom,

		NULL,								//parent window (this is a main one)
		NULL,								//menu
		mainInst,							//instance handle
		NULL								//additional app params
	);

	if (!mainWnd) return -1;				//error out if no window is created

	mainDC = GetDC(mainWnd);				//set main device context for the main window
	ShowWindow(mainWnd, nCmdShow);
	UpdateWindow(mainWnd);

	//create thread for tracking the mouse
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&XThread::TMouseTracking, NULL, NULL, NULL);

	//Window messaging and processing
	MSG msg;
	BOOL getResult;
	while ((getResult = GetMessage(&msg, NULL, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		MouseProc(&msg);			//process mouse messages in the MouseProc.cpp
	}

	//returns the defined close/destroy code
	if (getResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}