#ifndef VIEW_SYMBOL_TABLE_H
#define VIEW_SYMBOL_TABLE_H

#include "View.h"

#include <wx/listctrl.h>

class ViewSymbolTable : public View
{
public:
	ViewSymbolTable(ElfFile *file, wxString location);

protected:
	wxListCtrl *mListCtrl;
	long mSection;

	wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	wxString GetBindDescription(int bind);
	wxString GetTypeDescription(int type);
	wxString GetSectionDescription(int section);
};
#endif