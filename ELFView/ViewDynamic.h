#ifndef VIEW_DYNAMIC_H
#define VIEW_DYNAMIC_H

#include "ElfFile.h"
#include "View.h"

#include <wx/listctrl.h>

class ViewDynamic : public View
{
public:
	ViewDynamic(ElfFile *file, wxString location);

protected:
	wxListCtrl *mListCtrl;
	int mOffset;
	int mSize;

	wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
};

#endif