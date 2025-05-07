#include "main.h"
#include "rc.h"
#include <stdexcept>

int x = 8;
int y = 8;
COLORREF color = RGB(33, 33, 33);

COLORREF get_color(HWND parent) {
	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	static DWORD rgbCurrent = RGB(33, 33, 33); 

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = parent;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE) {
		rgbCurrent = cc.rgbResult;
	}

	return rgbCurrent;
}

int size_dialog::idd() const {
	return IDD_SIZE; 
}

bool size_dialog::on_init_dialog(){
	set_int(IDC_EDIT1, x);
	set_int(IDC_EDIT2, y);
	return true;
}

bool size_dialog::on_ok(){
	try {
		int new_x = get_int(IDC_EDIT1);
		int new_y = get_int(IDC_EDIT2);

		if (new_x <= 0 || new_y <= 0)
			throw std::runtime_error("Non-positive number");

		x = new_x;
		y = new_y;
	}
	catch (...) {
		MessageBox(*this, _T("Please enter positive numbers."), _T("Invalid Input"), MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}


void main_window::on_paint(HDC hdc) {
	HBRUSH brush = CreateSolidBrush(color);
	RECT clientRect;
	GetClientRect(*this, &clientRect);

	SetMapMode(hdc, MM_ANISOTROPIC);
	SetViewportExtEx(hdc, clientRect.right, clientRect.bottom, 0);
	SetWindowExtEx(hdc, x * 2, y * 2, 0);

	for (int i = 0; i < x * 2; i += 2) {
		for (int j = ((i / 2) % 2) * 2; j < y * 2; j += 4) {
			RECT r = { i, j, i + 2, j + 2 };
			FillRect(hdc, &r, brush);
		}
	}

	DeleteObject(brush);
}

void main_window::on_command(int id){
	switch(id){
		case ID_SIZE: {
			size_dialog s_dlg;
			if (s_dlg.do_modal(0, *this) == IDOK) {
				InvalidateRect(*this, NULL, true);
			}
			break;
		}
		case ID_COLOR:
			color = get_color(*this);
			InvalidateRect(*this, NULL, true);
			break;
		case ID_EXIT: 
			DestroyWindow(*this); 
			break;
	}
}

void main_window::on_destroy(){
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR, int)
{
	vsite::nwp::application app;
	main_window wnd;	
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (UINT_PTR)LoadMenu(hi, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
