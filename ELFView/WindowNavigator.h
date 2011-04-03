#ifndef WINDOW_NAVIGATOR_H
#define WINDOW_NAVIGATOR_H

#include <wx/notebook.h>
#include <wx/listbox.h>

class WindowNavigator : public wxNotebook
{
public:
	WindowNavigator(wxWindow *parent, wxWindowID id);

protected:
	wxListBox *mSectionList;
	wxListBox *mSegmentList;
};
#endif