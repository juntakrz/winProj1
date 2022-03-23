#pragma once

#ifndef UNICODE
#define UNICODE
#endif

//GENERIC DEFINITIONS FOR THE APP

//default app settings
#ifndef APP_DEFAULTS
#define APP_WINNAME L"Red Alert 2 Launcher"
#define APP_WINCLASS L"XWinClass"
#define APP_MAINRESX 800
#define APP_MAINRESY 600
#define APP_MAXCTL 16
#define APP_DEFBTNW 126
#define APP_DEFBTNH 25
#define APP_DEFFONT L"Tahoma"
#define APP_DEFFONTSIZE 16
#define APP_DEFTEXTCOLOR RGB(255, 255, 0)
#define APP_DEFBKCOLOR RGB(225, 0, 0)

#define INIFILE1 L".\\RA2.INI"
#define INIFILE2 L".\\RA2MD.INI"
#define APPPATH1 L".\\Ra2.exe"
#define APPPATH2 L".\\Yuri.exe"
#endif

//define control classes / CC (0 = no class)
#define CC_BUTTON 1
#define CC_CHECKBOX 2
#define CC_RADIOBOX 3
#define CC_LOGOBUTTON 4
#define CC_STATIC 5

//define control states
#define CS_DEFAULT 0
#define CS_HOVER 1
#define CS_CLICK 2

//define control names / IDC	(0 = null control name)
#define IDC_APPLY 1
#define IDC_BROWSE 2
#define IDC_EXIT 3

//define bitmaps
#define BTN_DEFAULT IDB_BITMAP2
#define BTN_HOVER IDB_BITMAP3
#define BTN_CLICK IDB_BITMAP4
#define BTN_CHK IDB_BITMAP7
#define BTN_UCHK IDB_BITMAP8
#define RA2LOGO IDB_BITMAP5
#define YRLOGO IDB_BITMAP6

//defining UI elements (deprecated in this program)
#define IDC_BUTN01 11
#define IDC_BUTN02 12
#define IDC_BUTN03 13
#define IDC_BUTN04 14
#define IDC_BUTN05 15
#define IDC_BUTN06 16
#define IDC_BUTN07 17
#define IDC_BUTN08 18
#define IDC_BUTN09 19
#define IDC_BUTN10 20

#define IDC_TEXT01 31

#define IDC_COMB01 71