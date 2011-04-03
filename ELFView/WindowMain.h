#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <wx/notebook.h>
#include <wx/htmllbox.h>

class WindowMain : public wxNotebook
{
public:
	WindowMain(wxWindow *parent, wxWindowID id);

protected:
	wxSimpleHtmlListBox *mElfHeader;
	wxSimpleHtmlListBox *mSectionHeaders;
	wxSimpleHtmlListBox *mProgramHeaders;
};

#endif