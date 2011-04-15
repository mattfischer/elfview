#ifndef VIEW_STRING_TABLE_H
#define VIEW_STRING_TABLE_H

#include "ElfFile.h"
#include "View.h"

#include "LinkTable.h"

#include <vector>

class ViewStringTable : public View
{
public:
	ViewStringTable(ElfFile *file, wxString location);

protected:
	LinkTable *mTable;
	long mSection;
	std::vector<int> mOffsets;

	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	virtual void doSetOffset(int offset);
};

#endif