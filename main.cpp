//------------------------------------
//  main.cpp
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "resource.h"
#include "main.h"

#include "winmaker.h"
#include "winex.h"
#include <new.h>

int WINAPI WinMain
    (HINSTANCE hInst, HINSTANCE hPrevInst, char * cmdParam, int cmdShow)
{
	if (_DEBUG) {
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		std::cout << ":: DEBUG ::" << std::endl;
	}

    _set_new_handler (&NewHandler);

    try
    {
        // Create top window class
        TopWinClass topWinClass (ID_MAIN, hInst, MainWndProc);
        // Is there a running instance of this program?
        HWND hwndOther = topWinClass.GetRunningWindow ();
        if (hwndOther != 0)
        {
			// thread into the foreground and activates the window			
            ::SetForegroundWindow (hwndOther);
			// this window is minimized ?
            if (::IsIconic (hwndOther))
                ::ShowWindow (hwndOther, SW_RESTORE);
            return 0;
        }
        topWinClass.Register ();

        // Create top window
        ResString caption (hInst, ID_CAPTION);
        TopWinMaker topWin (topWinClass, caption);
        topWin.Create ();
        topWin.Show (cmdShow);
        // The main message loop
        MSG  msg;
        int status;
        while ((status = ::GetMessage (&msg, 0, 0, 0)) != 0)
        {
            if (status == -1)
                return -1;
            ::TranslateMessage (&msg);
            ::DispatchMessage (&msg);
        }

        return msg.wParam;
    }
    catch ( WinException e )
    {
        char buf [50];
        wsprintf (buf, "%s, Error %d", e.GetMessage (), e.GetError ());
        ::MessageBox (0, buf, "Exception", MB_ICONEXCLAMATION | MB_OK);
    }
    catch (...)
    {
        ::MessageBox (0, "Unknown", "Exception", MB_ICONEXCLAMATION | MB_OK);
    }

    return 0;
}            

int NewHandler (size_t size)
{
    throw WinException ("Out of memory");
    return 0;
}

