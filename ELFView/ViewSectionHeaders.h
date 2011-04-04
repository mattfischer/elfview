#ifndef VIEW_SECTION_HEADERS_H
#define VIEW_SECTION_HEADERS_H

#include "View.h"

#include <wx/htmllbox.h>

class ViewSectionHeaders : public View
{
public:
	ViewSectionHeaders();

protected:
	wxSimpleHtmlListBox *mHtmlListBox;

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	virtual void doUpdateWindow();
};
#endif