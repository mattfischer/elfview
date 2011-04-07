#ifndef VIEW_RELOCATIONS_H
#define VIEW_RELOCATIONS_H

#include "ElfFile.h"
#include "View.h"

#include <wx/listctrl.h>

class ViewRelocations : public View
{
public:
	ViewRelocations(ElfFile *file, wxString location);

protected:
	wxListCtrl *mListCtrl;
	long mSection;
	wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
};

#endif