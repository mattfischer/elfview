#ifndef VIEW_STRING_TABLE_H
#define VIEW_STRING_TABLE_H

#include "ElfFile.h"
#include "View.h"

#include <wx/listctrl.h>

class ViewStringTable : public View
{
public:
	ViewStringTable(ElfFile *file, wxString location);

protected:
	wxListCtrl *mListCtrl;
	long mSection;
	wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
};

#endif