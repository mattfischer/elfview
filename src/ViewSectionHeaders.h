#ifndef VIEW_SECTION_HEADERS_H
#define VIEW_SECTION_HEADERS_H

#include "View.h"
#include "LinkTable.h"

class ViewSectionHeaders : public View
{
public:
	ViewSectionHeaders(ElfFile *file, wxString location);

protected:
	LinkTable *mTable;

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	virtual void doSetOffset(int offset);
};
#endif