#if !defined CONTROL_H
#define CONTROL_H
//------------------------------------
//  control.h
//  Controller
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "view.h"
#include "model.h"
#include <windows.h>

enum TYPE_BYTTON { press, hover, leave };

class Controller
{
public:
    Controller(HWND hwnd, CREATESTRUCT * pCreate);
    ~Controller ();
    void    Size (int x, int y);
    void    Paint ();
    void    Command (int cmd);
	void	BtnCollor (HWND comp, HWND cur, DRAWITEMSTRUCT* dis, UINT message);
	void	DrawItems (LPARAM lParam, HWND item, UINT message);
	void	Test(HWND hwndbtn);
	void	DrawButton(HWND* hwnd_item, DRAWITEMSTRUCT* dis, RECT& rcBtnSend, TYPE_BYTTON type);
	void	DrawElsBtn(HWND* hwnd_item, DRAWITEMSTRUCT* dis, RECT& rcBtnSend);
private:

    HWND        _hwnd;

    Model       _model;
    View        _view;

	HBRUSH		_Border;
	HBRUSH		_Area;
};

#endif

