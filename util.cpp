#include "main.h"

int iResArray[32][2];
int iINIParam[3] = { 0,0,0 };

void GetDesktopResolution(int &horizontal, int &vertical)
{
	//variable to store monitor dimensions
	RECT desktop;

	//get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();

	//move desktop coordinates into RECT variable
	GetWindowRect(hDesktop, &desktop);

	horizontal = desktop.right;
	vertical = desktop.bottom;

}

RECT InitWinCoordinates(const DWORD& WinStyle, const bool& AdjClientArea) {

	RECT rFullWin = { 0, 0, APP_MAINRESX, APP_MAINRESY };
	if (AdjClientArea) AdjustWindowRectEx(&rFullWin, WinStyle, false, 0);	//either client area or full window will be of app size
	int initPosX, initPosY;
	int winResX = rFullWin.right - rFullWin.left, winResY = rFullWin.bottom - rFullWin.top;	//variables for window resolution
	GetDesktopResolution(initPosX, initPosY);
	initPosX = (initPosX / 2) - (winResX / 2);			//create window in the center of the desktop
	initPosY = (initPosY / 2) - (winResY / 2);

	return rFullWin = { initPosX, initPosY, winResX, winResY };		//x, y, w, h

}

void GPUResolutionsToArray() {

	int posX = 0;			//starting position in an array

	DEVMODE dm;
	dm.dmSize = sizeof(dm);

	int prevRW = 0, prevRH = 0;				//compare reported resolutions to avoid filling array with duplicates

	for (int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm); ++iModeNum) {			//fill array with resolutions
		if (((prevRW != dm.dmPelsWidth) || (prevRH != dm.dmPelsHeight)) && (posX < 32)) {

			iResArray[posX][0] = dm.dmPelsWidth;
			iResArray[posX][1] = dm.dmPelsHeight;
			posX++;
		}
		prevRW = dm.dmPelsWidth;
		prevRH = dm.dmPelsHeight;
	}
	
	//sorting resolutions
	int iNewX = 0, iNewY = 0, iOldX = 0, iOldY = 0;

	for (int i = 0; i < posX + 1; i++) {			//go through all positions of resolution array

		for (int a = i; a < posX + 1; a++) {		//go through all positions of resolution array again
			
			iNewX = iResArray[i][0];					//put vars at current position into temporary vars
			iNewY = iResArray[i][1];

			if ((iNewX != iResArray[a][0] || iNewY != iResArray[a][1]) && iResArray[a][0])	//do the following only if currently checked resolution isn't the same
			{

				if ((iNewX >= iResArray[a][0] && iNewY > iResArray[a][1]) || iNewX > iResArray[a][0]) {	//if width is larger or equal and height is larger - switch positions

					iOldX = iResArray[a][0];
					iOldY = iResArray[a][1];

					iResArray[a][0] = iNewX;
					iResArray[a][1] = iNewY;
					iResArray[i][0] = iOldX;
					iResArray[i][1] = iOldY;
				}
			}

		}

	}
}

std::wstring GetMonitorData() {

	//Getting monitor data
	auto monitor{ MonitorFromWindow(GetConsoleWindow(), MONITOR_DEFAULTTOPRIMARY) };
	MONITORINFOEXW miex{};
	miex.cbSize = sizeof(MONITORINFOEXW);  // set cbSize member
	GetMonitorInfoW(monitor, &miex);

	DEVMODEW dm;
	memset(&dm, 0, sizeof(DEVMODE));
	if (!EnumDisplaySettings(NULL, 0, &dm)) return L"HW: ERROR 1";

	DISPLAY_DEVICEW dd = { sizeof(dd), 0 };
	//retrieve current display ID
	if (!EnumDisplayDevices(NULL, 0, &dd, NULL)) return L"HW: ERROR 3";
	std::wstring tempDev = dd.DeviceName;
	//use it to retrieve display name
	if (!EnumDisplayDevices(tempDev.c_str(), 0, &dd, NULL)) return L"HW:ERROR 4";
	std::wstring monitorName = dd.DeviceString;
	//retrieve GPU used
	if (!EnumDisplayDevices(NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME)) return L"HW: ERROR 2";

	wchar_t wcDataStr[100] = {0};
	wsprintfW(wcDataStr, L"HW: %ls\\%ls: %d x %d %d Hz \\ %ls", miex.szDevice, monitorName.c_str(), miex.rcMonitor.right, miex.rcMonitor.bottom, dm.dmDisplayFrequency, dd.DeviceString);
	return wcDataStr;
}

void GetINIParameters(LPCWSTR INIPath) {

	//compare INI file and array resolution and return selection
	int compareW = GetPrivateProfileIntW(L"Video", L"ScreenWidth", NULL, INIPath);
	int compareH = GetPrivateProfileIntW(L"Video", L"ScreenHeight", NULL, INIPath);
	for (int i = 0; i < (*(&iResArray + 1) - iResArray); ++i)
		if (iResArray[i][0] == compareW && iResArray[i][1] == compareH && iResArray[i][0] != 0 && iResArray[i][1] != 0) iINIParam[0] = i;

	//play or skip intro movies
	wchar_t* paramStr = new wchar_t[8];
	GetPrivateProfileStringW(L"Intro", L"Play", L"yes", paramStr, sizeof(paramStr), INIPath);
	if (!wcscmp(paramStr, L"yes")) iINIParam[1] = 0;
	else iINIParam[1] = 1;

	//strech movies to screen
	GetPrivateProfileStringW(L"Video", L"StretchMovies", L"no", paramStr, sizeof(paramStr), INIPath);
	if (!wcscmp(paramStr, L"no")) iINIParam[2] = 0;
	else iINIParam[2] = 1;

	delete[] paramStr;
}

void SetINIParameters(LPCWSTR INIPath) {

	//if "Desktop" resolution button is checked - save that resolution to INI
	if (IsDlgButtonChecked(mainWnd, IDC_BUTN05)) {

		int resW, resH;
		GetDesktopResolution(resW, resH);
		for (int i = 0; i < (*(&iResArray + 1) - iResArray); ++i)
			if (iResArray[i][0] == resW && iResArray[i][1] == resH && iResArray[i][0] != 0 && iResArray[i][1] != 0) iINIParam[0] = i;
	}

	iINIParam[1] = IsDlgButtonChecked(mainWnd, IDC_BUTN07);
	iINIParam[2] = IsDlgButtonChecked(mainWnd, IDC_BUTN08);

	wchar_t* temp = new wchar_t[5];
	wsprintfW(temp, L"%d", iResArray[iINIParam[0]][0]);
	WritePrivateProfileStringW(L"Video", L"ScreenWidth", temp, INIPath);
	wsprintfW(temp, L"%d", iResArray[iINIParam[0]][1]);
	WritePrivateProfileStringW(L"Video", L"ScreenHeight", temp, INIPath);
	delete[] temp;

	if (iINIParam[1] == 0) WritePrivateProfileStringW(L"Intro", L"Play", L"yes", INIPath);
	else WritePrivateProfileStringW(L"Intro", L"Play", L"no", INIPath);

	if (iINIParam[2] == 0) WritePrivateProfileStringW(L"Video", L"StretchMovies", L"no", INIPath);
	else WritePrivateProfileStringW(L"Video", L"StretchMovies", L"yes", INIPath);
}

void LaunchApp(LPCWSTR Path) {
	
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	CreateProcessW(Path, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}