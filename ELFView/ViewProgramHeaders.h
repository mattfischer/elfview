#ifndef VIEW_PROGRAM_HEADERS_H
#define VIEW_PROGRAM_HEADERS_H

#include "View.h"

#include <wx/htmllbox.h>

class ViewProgramHeaders : public View
{
public:
	ViewProgramHeaders(ElfFile *file, wxString location);

protected:
	wxSimpleHtmlListBox *mHtmlListBox;

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
};
#endif