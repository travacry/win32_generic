#if !defined CANVAS_H
#define CANVAS_H
//------------------------------------
//  canvas.h
//  Graphics
//  (c) Reliable Software, 1997, 98
//------------------------------------

#include <windows.h>

// Encapsulate Windows Device Context
// This is an abstract class in virtue of a protected constructor

class Canvas
{
public:
    operator HDC () { return _hdc; }

    void Line (int x1, int y1, int x2, int y2)
    {
        ::MoveToEx (_hdc, x1, y1, 0);
        ::LineTo (_hdc, x2, y2);
    }

    void Text (int x, int y, char const * buf, int cBuf)
    {
        ::TextOut ( _hdc, x, y, buf, cBuf );
    }

    void Char (int x, int y, char c)
    {
        ::TextOut (_hdc, x, y, &c, 1);
    }

	HWND Button (int x, int y, int w, int h, char* c, int id_hmenu)
	{
		HWND hWnd = ::GetActiveWindow();
		HINSTANCE hInstance = GetModuleHandle(0);

		return ::CreateWindowEx(0,"BUTTON",c,  WS_VISIBLE | WS_CHILD  | BS_CENTER |BS_OWNERDRAW , 
			x, y, w, h, hWnd, (HMENU)id_hmenu,
			hInstance, NULL);
		//BS_OWNERDRAW
	}
    //
    // Keep adding new methods as needed
    //

protected:

    Canvas (HDC hdc): _hdc (hdc) {}

    HDC  _hdc;
};


// Use for painting in response to WM_PAINT

class PaintCanvas: public Canvas
{
public:
    PaintCanvas (HWND hwnd)
        : Canvas (::BeginPaint (hwnd, &_paint)),
          _hwnd (hwnd)
    {}

    ~PaintCanvas ()
    {
        ::EndPaint(_hwnd, &_paint);
    }
 
protected:
                 
    PAINTSTRUCT _paint;
    HWND        _hwnd;
};

#endif
