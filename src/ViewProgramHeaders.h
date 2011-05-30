#ifndef VIEW_PROGRAM_HEADERS_H
#define VIEW_PROGRAM_HEADERS_H

#include "View.h"

#include "LinkTable.h"

class ViewProgramHeaders : public View
{
public:
	ViewProgramHeaders(ElfFile *file, wxString location);

protected:
	LinkTable *mTable;

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	virtual void doSetOffset(int offset);
};
#endif