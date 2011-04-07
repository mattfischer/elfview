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

protected:

	void OnViewAdded(wxCommandEvent &e);
	void OnViewRemoved(wxCommandEvent &e);
	void OnCurrentViewChanged(wxCommandEvent &e);

	ViewManager *mViewManager;
	ElfFile *mFile;
};
#endif