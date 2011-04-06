#ifndef WINDOW_NAVIGATOR_H
#define WINDOW_NAVIGATOR_H

#include <wx/notebook.h>
#include <wx/listbox.h>

#include "ElfFile.h"
#include "ViewManager.h"

class WindowNavigator : public wxNotebook
{
public:
	WindowNavigator(wxWindow *parent, wxWindowID id, ViewManager *viewManager);

	void SetFile(ElfFile *file);

protected:
	ElfFile *mFile;
	ViewManager *mViewManager;
	wxListBox *mSectionList;
	wxListBox *mSegmentList;

	void OnSectionSelected(wxCommandEvent &e);
	void OnSegmentSelected(wxCommandEvent &e);

	DECLARE_EVENT_TABLE();
};
#endif