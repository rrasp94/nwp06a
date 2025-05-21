#include <windows.h>
#include <tchar.h>
#include "nwpwin.h"
#include "nwpdlg.h"

class size_dialog : public vsite::nwp::dialog {
public:
	int x = 8;
	int y = 8;

protected:
	int idd() const override;
	bool on_init_dialog() override;
	bool on_ok() override;
};

class main_window : public vsite::nwp::window {
private:
	int x = 8;
	int y = 8;
	COLORREF color = RGB(33, 33, 33);

protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;

public:
	COLORREF get_color(HWND parent, COLORREF current_color);
};
