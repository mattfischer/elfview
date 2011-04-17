#ifndef VIEW_ELF_HEADER_H
#define VIEW_ELF_HEADER_H

#include "View.h"
#include "LinkTable.h"

class ViewElfHeader : public View
{
public:
	ViewElfHeader(ElfFile *file, wxString location);

protected:
	LinkTable *mTable;

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	virtual void doSetOffset(int offset);
};
#endif