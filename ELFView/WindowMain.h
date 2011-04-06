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
	WindowMain(wxWindow *parent, wxWindowID id, ViewManager *viewManager);

	void SetFile(ElfFile *file);

	void AddViewPage(wxWindow *window, wxString name);
	void SwitchToViewPage(int page);
	void RemoveViewPage(int page);
protected:
	ViewManager *mViewManager;
	ElfFile *mFile;
};
#endif