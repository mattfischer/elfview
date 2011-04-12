#ifndef VIEW_SYMBOL_TABLE_H
#define VIEW_SYMBOL_TABLE_H

#include "View.h"

#include "LinkTable.h"

class ViewSymbolTable : public View
{
public:
	ViewSymbolTable(ElfFile *file, wxString location);

protected:
	LinkTable *mTable;
	long mSection;

	wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
};
#endif