#pragma once

//link to ComCtl32.dll
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "Msimg32.lib")

#include <windows.h>
#include <sstream>
#include <CommCtrl.h>
#include "appdef.h"
#include "resource.h"

//GLOBAL VARIABLES
//main.cpp
extern HWND mainWnd;				//handle to main window
extern HINSTANCE mainInst;			//handle to main instance
extern HDC bDC, mainDC;				//handle to buffer and main device context
extern RECT clRect;					//useable window area
extern BOOL bChangesMade;			//if any changes were made without saving

//util.cpp
extern int iResArray[32][2];
extern int iINIParam[3];			//0 = current res selection, 1 = skip intro, 2 = strech movies

//GLOBAL FUNCTIONS

//MouseProc.cpp
void MouseProc(MSG* pMsg);

//paint.cpp			---WM_PAINT etc. handling
void DrawFrame();

//UIScripts.cpp		--- creation of Windows GUI elements happens here
void CreateUIElement(HWND hWnd, int IDC, LPCWSTR UIText, int posX, int posY, int sizeW = 0, int sizeH = 0, int ParamA = 0, int ParamB = 0);
void CreateUIMain(HWND hWnd);									//creates GUI elements and assigns IDs to them
void PopulateComboBox(HWND hWnd, int Option, int ParamA = 0);	//populate combobox based on the checkbox selection

//util.cpp				--- additional helpful functions for the main code
void GetDesktopResolution(int &horizontal, int &vertical);		//gets desktop resolution
RECT InitWinCoordinates(const DWORD& WinStyle, const bool& AdjClientArea);	//calculates proper window size and position
void GPUResolutionsToArray();									//fills resArray with available resolutions
std::wstring GetMonitorData();										//returns monitor+GPU string of data
void GetINIParameters(LPCWSTR INIPath);
void SetINIParameters(LPCWSTR INIPath);
void LaunchApp(LPCWSTR FilePath);

template<typename T>											//converts LONG to wstring and later LPWSTR
std::wstring toString(const T& value)
{
	std::wostringstream woss;
	woss << value;
	return woss.str();
}