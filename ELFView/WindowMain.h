#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <wx/notebook.h>
#include <wx/htmllbox.h>
#include <wx/list.h>

#include "ElfFile.h"
#include "View.h"
#include "ViewManager.h"

class WindowMain : public wxNotebook
{
public:
	WindowMain(wxWindow *parent, wxWindowID id);

	void SetFile(ElfFile *file);

	void AddWindow(wxWindow *window, wxString name);
	void SwitchToWindow(int window);

protected:
	ViewManager *mViewManager;
};
#endif