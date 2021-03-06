#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <wx/notebook.h>
#include <wx/htmllbox.h>
#include <wx/list.h>
#include <wx/hyperlink.h>

#include "View.h"
#include "ViewManager.h"

class WindowMain : public wxNotebook
{
public:
	WindowMain(wxWindow *parent, wxWindowID id, ViewManager *viewManager);

	void GoToLocation(wxString location);

protected:

	void OnViewAdded(wxCommandEvent &e);
	void OnViewRemoved(wxCommandEvent &e);
	void OnCurrentViewChanged(wxCommandEvent &e);

	void OnRightUp(wxMouseEvent &e);
	void OnMenu(wxCommandEvent &e);	

	ViewManager *mViewManager;
	int mCloseTab;

	DECLARE_EVENT_TABLE();
};
#endif