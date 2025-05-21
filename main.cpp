#include "main.h"
#include "rc.h"
#include <stdexcept>

COLORREF main_window::get_color(HWND parent, COLORREF current_color) {
	CHOOSECOLOR cc = {};
	COLORREF acrCustClr[16] = {};

	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = parent;
	cc.lpCustColors = acrCustClr;
	cc.rgbResult = current_color;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE) {
		return cc.rgbResult;
	}

	return current_color;
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
	SetWindowExtEx(hdc, x, y, 0);

	for (int i = 0; i < x; ++i) {
		for (int j = (i % 2); j < y; j += 2) {
			RECT r = { i, j, i + 1, j + 1 };
			FillRect(hdc, &r, brush);
		}
	}

	DeleteObject(brush);
}

void main_window::on_command(int id) {
	switch (id) {
	case ID_SIZE: {
		size_dialog dlg;
		dlg.x = x;
		dlg.y = y;
		if (dlg.do_modal(0, *this) == IDOK) {
			x = dlg.x;
			y = dlg.y;
			InvalidateRect(*this, NULL, true);
		}
		break;
	}
	case ID_COLOR:
		color = get_color(*this, color);
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
