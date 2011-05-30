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

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	virtual void doSetOffset(int offset);
};
#endif