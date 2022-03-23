#include "main.h"
#include "Threads.h"
#include "XCtl.h"

static bool bTrackMouse = true, bRedraw = true;

void XThread::TMouseTracking() {

	wchar_t debug[40];
	POINT pMousePos = { 0 };
	RECT rCtlRect = { 0 };
	unsigned char LastIDC = 0;
	bool bIsHovering = false;

	while (bTrackMouse) {

		GetCursorPos(&pMousePos);								//getting mouse position
		ScreenToClient(mainWnd, &pMousePos);					//and constraining coordinates to the window space

		for (unsigned char CurrentIDC = 0; CurrentIDC < APP_MAXCTL + 1; CurrentIDC++) {
			if (CurrentIDC == APP_MAXCTL) {						//there are only 0-63 virtual controls
				OutputDebugStringW(L"LEFT!\n");					//so at 64 cursor isn't over any of them
				bIsHovering = false;
				break;
			}

			if (LastIDC != CurrentIDC && bIsHovering) {			//cursor stopped hovering over the previous virtual control
				XCtl::SetCtlState(LastIDC, CS_DEFAULT);
				bIsHovering = false;
			}

			if (XCtl::GetCtlClass(CurrentIDC) > 0) {
				XCtl::GetCtlPos(CurrentIDC, rCtlRect);
				if (pMousePos.x > rCtlRect.left && pMousePos.x < rCtlRect.right && pMousePos.y > rCtlRect.top && pMousePos.y < rCtlRect.bottom) {

					wsprintfW(debug, L"Mouse is over IDC = %d\nLast active IDC = %d\n", CurrentIDC, LastIDC);
					OutputDebugStringW(debug);

					XCtl::SetCtlState(CurrentIDC, CS_HOVER);
					LastIDC = CurrentIDC;
					bIsHovering = true;			//cursor is over the virtual control, tell that to the app
					break;
				}
			}
		}

		Sleep(1000 / 24);						//track at just 24 samples per second, should be enough
	}

}