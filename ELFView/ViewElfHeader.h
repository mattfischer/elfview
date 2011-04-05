#ifndef VIEW_ELF_HEADER_H
#define VIEW_ELF_HEADER_H

#include "View.h"

#include <wx/htmllbox.h>

class ViewElfHeader : public View
{
public:
	ViewElfHeader(ElfFile *file, wxString location);

protected:
	wxSimpleHtmlListBox *mHtmlListBox;

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
};
#endif