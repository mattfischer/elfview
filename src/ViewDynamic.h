#ifndef VIEW_DYNAMIC_H
#define VIEW_DYNAMIC_H

#include "ElfFile.h"
#include "View.h"

#include "LinkTable.h"

class ViewDynamic : public View
{
public:
	ViewDynamic(ElfFile *file, wxString location);

protected:
	LinkTable *mTable;
	int mOffset;
	int mSize;

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	virtual void doSetOffset(int offset);
};

#endif