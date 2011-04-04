#ifndef VIEW_ELF_HEADER_H
#define VIEW_ELF_HEADER_H

#include "View.h"

#include <wx/htmllbox.h>

class ViewElfHeader : public View
{
public:
	ViewElfHeader();

protected:
	wxSimpleHtmlListBox *mHtmlListBox;

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	virtual void doUpdateWindow();
};
#endif