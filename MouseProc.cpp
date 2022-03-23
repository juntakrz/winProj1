#include "main.h"
#include "XClasses.h"

void MouseProc(MSG* pMsg)
{
	static bool isTracking = false;
	int CtlID = GetDlgCtrlID(pMsg->hwnd);		//get ID of the control

	switch (pMsg->message)
	{
	case  WM_MOUSEMOVE:
	{
		if (!isTracking) {
			TRACKMOUSEEVENT tme = {};
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.hwndTrack = pMsg->hwnd;
			tme.dwHoverTime = 10; //HOVER_DEFAULT;
			TrackMouseEvent(&tme);
		}
		isTracking = true;

		break;
	}

	case WM_MOUSEHOVER:
	{
		if (CtlID >= 11 && CtlID <= 15) {
			CButtonProc::OnHover(pMsg->hwnd, CtlID);
		}
		break;
	}

	case WM_MOUSELEAVE:
	{
		isTracking = false;

		if (CtlID >= 11 && CtlID <= 15) {
			CButtonProc::OnLeave(pMsg->hwnd, CtlID);
		}
		break;

	}

	case WM_LBUTTONDOWN: {

		if (CtlID >= 11 && CtlID <= 15) {
			CButtonProc::OnClick(pMsg->hwnd, CtlID, TRUE);
		}
		isTracking = false;
		break;
	}

	case WM_LBUTTONDBLCLK: {

		if (CtlID >= 11 && CtlID <= 15) {
			CButtonProc::OnClick(pMsg->hwnd, CtlID, TRUE, TRUE);
		}
		isTracking = false;
		break;
	}

	case WM_LBUTTONUP: {

		if (CtlID >= 11 && CtlID <= 15) {
				CButtonProc::OnClick(pMsg->hwnd, CtlID);
		}
		isTracking = false;
		break;

	}

	}
}