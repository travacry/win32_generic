//------------------------------------
//  control.cpp
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "control.h"
#include "canvas.h"
#include "main.h"
#include "model.h"
#include "resource.h"
#include "winex.h"
#include "winmaker.h"
#include <WinUser.h>

#define WM_MOUSE_BTN_ENTER 100001
#define WM_MOUSE_BTN_LEAVE 100002


// Window Procedure

LRESULT CALLBACK MainWndProc
    (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TRACKMOUSEEVENT msevnt1;
	static TRACKMOUSEEVENT msevnt2;
	static bool onPaint = false;
	static HWND bntTest = NULL;
	HCURSOR cursors[10];
	cursors[0] = LoadCursor(NULL, IDC_ARROW);    // default cursor
	cursors[1] = LoadCursor(NULL, IDC_CROSS);    // other cursor
	cursors[2] = LoadCursor(NULL, IDC_WAIT);     // wait cursor
	cursors[3] = LoadCursor(NULL, IDC_SIZEWE);
	cursors[4] = LoadCursor(NULL, IDC_SIZENS);
    // Pointer to Controller is stored in Window
    Controller * pCtrl = WinGetLong<Controller *> (hwnd);

	//WTF magik ?
	int item = (int)(LOWORD(wParam));
	int curItem = (int)GetDlgItem( hwnd, IDC_BTN_TEST );
	int btn_  = (int)bntTest;

	static bool onSetCursor = false;
    switch (message)
    {
	case WM_DRAWITEM:
		if (LOWORD(hwnd) == IDC_BTN_TEST || onSetCursor)
		{
			onSetCursor = false;
			pCtrl->DrawItems (lParam, (HWND)wParam, message);
		} else {
			::DefWindowProc( hwnd, message, lParam, wParam );
		}
		return 0;
	case WM_SETCURSOR:		
		LPRECT rect;
		::GetClientRect(hwnd, rect);
		if( (HWND)wParam == bntTest ) 
		{
			onSetCursor = true;
			pCtrl->DrawItems (lParam, bntTest, message);	
			return TRUE;
		} else if ((HWND)wParam == hwnd) {
			onSetCursor = true;
			pCtrl->DrawItems (lParam, hwnd, message);
			return TRUE;
		}
        else
			return ::DefWindowProc( hwnd, message, lParam, wParam );
    case WM_CREATE:
        try
        {
            pCtrl = new Controller (hwnd, reinterpret_cast<CREATESTRUCT *> (lParam));
            // Store pointer to Controller inside Window (application)
            WinSetLong<Controller *> (hwnd, pCtrl);
        }
        catch (WinException e)
        {
            ::MessageBox (hwnd, e.GetMessage(), "Initialization", 
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        catch (...)
        {
            ::MessageBox (hwnd, "Unknown Error", "Initialization", 
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        return 0;
    case WM_SIZE:
        pCtrl->Size (LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_PAINT:
		//if (!onPaint)
		//{
			onPaint = true;		
			::SetCursor(cursors[0]);
			pCtrl->Paint ();
			std::cout<<"paint"<<std::endl;
			pCtrl->DrawItems (lParam, (HWND)wParam, message);

			bntTest = ::GetDlgItem( hwnd, IDC_BTN_TEST );

			pCtrl->DrawItems (lParam, hwnd, message);
		//}
        return 0;
    case WM_COMMAND:
        pCtrl->Command (LOWORD (wParam));
        return 0;
    case WM_DESTROY:
        WinSetLong<Controller *> (hwnd, 0);
        delete pCtrl;
        return 0;
    }
    return ::DefWindowProc (hwnd, message, wParam, lParam);
}
/*
	case WM_DRAWITEM:
	if (LOWORD(hwnd) == IDC_BTN_TEST || onSetCursor)
	{
		onSetCursor = false;
		pCtrl->DrawItems (lParam, (HWND)wParam, message);
	} else {
		DefWindowProc( hwnd, message, lParam, wParam );
	}
	return 0;
	case WM_SETCURSOR:
		onSetCursor = true;
		LPRECT rect;
		GetClientRect(hwnd, rect);
		if( (HWND)wParam == bntTest ) 
		{				
			pCtrl->DrawItems (lParam, bntTest, message);	
			return TRUE;
		} else if ((HWND)wParam == hwnd) {
			pCtrl->DrawItems (lParam, hwnd, message);
			return TRUE;
		}
        else
			return DefWindowProc( hwnd, message, lParam, wParam );
*/
void Controller::Test(HWND hwndbtn)
{
	HWND hButt_ = _view.getBtnTest();

	if (hButt_ == hwndbtn)
		std::cout<<"this btn"<<std::endl;
}

Controller::Controller (HWND hwnd, CREATESTRUCT * pCreate)
   :_hwnd (hwnd), 
    _model ("Generic") 
{
}

Controller::~Controller()
{
    ::PostQuitMessage(0);
}

void Controller::Size (int cx, int cy)
{
    _view.SetSize (cx, cy);
}

void Controller::Paint ()
{	
    PaintCanvas canvas (_hwnd);
    _view.Paint (canvas, _model);
}

void Controller::DrawElsBtn(HWND* hwnd_item, DRAWITEMSTRUCT* dis, RECT& rcBtnSend)
{
	std::cout<<"hwnd : "<<hwnd_item<<std::endl;
	std::cout<<"dis : "<<dis<<std::endl;
/*	
	::FillRect(dis->hDC,&rcBtnSend,_Area);
	::DeleteObject(_Area);	

	rcBtnSend.top += 5; rcBtnSend.bottom -= 5;
	rcBtnSend.left += 5; rcBtnSend.right -= 5;
	::FillRect(dis->hDC,&rcBtnSend,_Border);
	::DeleteObject(_Border);	
*/		
	TCHAR buffer[MAX_PATH];
	::ZeroMemory(buffer,MAX_PATH);
	HFONT hfontArial;
	HGDIOBJ oldfont;
	
	::SetBkMode(dis->hDC,TRANSPARENT);
	::GetWindowText(dis->hwndItem,buffer,MAX_PATH);	
	hfontArial = ::CreateFont (16, 0, FW_DONTCARE, FW_DONTCARE, FW_DONTCARE,
        FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	::SetTextColor (dis->hDC, RGB(0, 0, 0));
	oldfont = ::SelectObject (dis->hDC, hfontArial);

	::DrawText(dis->hDC, buffer, strlen(buffer), &rcBtnSend, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	::DeleteObject (hfontArial);
	
	::ReleaseDC(dis->hwndItem, dis->hDC);
}

void Controller::DrawButton(HWND* hwnd_item, DRAWITEMSTRUCT* dis, RECT& rcBtnSend, TYPE_BYTTON type)
{
	//enum TYPE_BYTTON { press, hover, leave };
	HDC hDc = dis->hDC;

	COLORREF Text;
	SetTextColor(dis->hDC,RGB(0,255,255));	
	static bool leave_btn = false;
	switch(type)
	{
	case press:
		leave_btn = false;
		_Area=CreateSolidBrush(RGB(0,0,0));
		_Border=CreateSolidBrush(RGB(170,227,255));	
		//	
		break;
	case hover:
		leave_btn = false;
		_Area=CreateSolidBrush(RGB(195,195,195));
		_Border=CreateSolidBrush(RGB(255,255,255));	
		//SetTextColor(dis->hDC,RGB(0,255,255));		
		break;
	case leave:
		Text=RGB(255,0,0);
		if (!leave_btn)
		{
			leave_btn = true;
			_Area=CreateSolidBrush(RGB(0,0,0));
			_Border=CreateSolidBrush(RGB(255,0,255));				
		} else {
			return;	
		}
		break;
	default:
		return;
	}

	DrawElsBtn(hwnd_item, dis,rcBtnSend);
}

void Controller::BtnCollor (HWND compare, HWND source, DRAWITEMSTRUCT* dis, UINT message)
{		
	RECT rcBtnSend;
	static HWND cur_btn = GetDlgItem( _hwnd, IDC_BTN_TEST );
	static TYPE_BYTTON state = leave;
	GetClientRect(cur_btn,&rcBtnSend);	

	//WM_SETCURSOR		
	DRAWITEMSTRUCT btnds;
	btnds.CtlID = IDC_BTN_TEST;
	btnds.CtlType = ODT_BUTTON;
	btnds.hDC = GetDC(cur_btn);
	btnds.hwndItem = cur_btn;
	btnds.itemAction = ODA_FOCUS;
	btnds.itemData = 0;
	btnds.itemID = IDC_BTN_TEST;
	btnds.itemState = ODS_DEFAULT;
	btnds.rcItem = rcBtnSend;


	static HWND source1 = _view.getBtnTest();

	switch (message)
	{
		case WM_DRAWITEM :	
			if (dis->itemState & ODS_SELECTED){
				//press
				DrawButton(&source1, dis, rcBtnSend, press);
			} else if (state == hover) {
				//hover
				DrawButton(&source1, dis, rcBtnSend, hover);
			} else if (state == leave) {
				//main
				DrawButton(&source1, dis, rcBtnSend, leave);				
			} 
				//
			break;
		case WM_PAINT:
			//LEAVE
			state = leave;
			SendMessage(_hwnd, WM_DRAWITEM, IDC_BTN_TEST, (LPARAM)&btnds);
			return;
	}
	
	if (message == WM_SETCURSOR)
	{ 
		if (compare == cur_btn)
		{
			//HOVER
			state = hover;
			SendMessage(_hwnd, WM_DRAWITEM, IDC_BTN_TEST, (LPARAM)&btnds);
			//DrawButton(dis, rcBtnSend, press);
			return;
		} else {
			//LEAVE
			state = leave;
			SendMessage(_hwnd, WM_DRAWITEM, IDC_BTN_TEST, (LPARAM)&btnds);
			//DrawButton(dis, rcBtnSend, leave);
			return;
		}
	}

}

void Controller::DrawItems (LPARAM lParam, HWND item, UINT message)
{	
	static HWND cur_btn = GetDlgItem( _hwnd, IDC_BTN_TEST );	
	DRAWITEMSTRUCT *Dis = (DRAWITEMSTRUCT*)lParam;	
	
	int cmd = LOWORD(item);
	//if (item != _view.getBtnTest())
	//	DefWindowProc( _hwnd, message, lParam, (LPARAM)&Dis );
	BtnCollor(item, _view.getBtnTest(), Dis, message);
}


/////////////////////// Only click /////////////////////////

/*
void Controller::BtnCollor (HWND comp, HWND cur, DRAWITEMSTRUCT* dis, UINT message)
{	
	RECT rcBtnSend;
	GetClientRect(cur,&rcBtnSend);	
	switch (message)
	{
		case WM_DRAWITEM :	
			if (dis->itemState & ODS_SELECTED){
				HBRUSH hBrushBorder=CreateSolidBrush(RGB(0,255,0));
				FillRect(dis->hDC,&rcBtnSend,hBrushBorder);
				DeleteObject(hBrushBorder);
			} else {
				HBRUSH White=CreateSolidBrush(RGB(0,0,0));
				HBRUSH Black=CreateSolidBrush(RGB(255,255,255));
				FillRect(dis->hDC,&rcBtnSend,White);
				DeleteObject(White);	

				rcBtnSend.top += 5; rcBtnSend.bottom -= 5;
				rcBtnSend.left += 5; rcBtnSend.right -= 5;
				FillRect(dis->hDC,&rcBtnSend,Black);
				DeleteObject(Black);	
				
				TCHAR buffer[MAX_PATH];
				ZeroMemory(buffer,MAX_PATH);
				::SetBkMode(dis->hDC,TRANSPARENT);
				::GetWindowText(dis->hwndItem,buffer,MAX_PATH);
				::DrawText(dis->hDC,buffer,strlen(buffer),&rcBtnSend, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			}
			break;
	}
}

void Controller::DrawItems (LPARAM lParam, HWND item, UINT message)
{	
	DRAWITEMSTRUCT *Dis = (DRAWITEMSTRUCT*)lParam;
	int cmd = LOWORD(item);
	
	switch (cmd)
	{
	case IDC_BTN_TEST:		
		BtnCollor(item, _view.getBtnTest(), Dis, message);
		break;
	}
}
*/

/////////////////////// Only click /////////////////////////

// Menu commands processing

void Controller::Command (int cmd)
{
    switch (cmd)
    {
	case IDC_BTN_TEST:	
		std::cout << ":: IDC_BTN_TEST :: Command" << std::endl;
		break;
    case IDM_EXIT:
        ::SendMessage (_hwnd, WM_CLOSE, 0, 0L);
        break;
    case IDM_HELP:
        ::MessageBox (_hwnd, "Go figure!",
            "Generic", MB_ICONINFORMATION | MB_OK);
        break;
    case IDM_ABOUT:
        {
            // Instance handle is available through HWND
            HINSTANCE hInst = WinGetLong<HINSTANCE> (_hwnd, GWL_HINSTANCE);
            ::DialogBox (hInst, 
                         MAKEINTRESOURCE (IDD_ABOUT), 
                         _hwnd, 
                         AboutDlgProc);
        }
        break;
    }
}

// "About" dialog box procedure
// Process messages from dialog box
// Caution: use Windows BOOL, not C++ bool

BOOL CALLBACK AboutDlgProc
   (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:		
		std::cout<<"WM_CREATE"<<std::endl;
		break;
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD (wParam))
        {
        case IDOK:
        case IDCANCEL:
            ::EndDialog (hwnd, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

