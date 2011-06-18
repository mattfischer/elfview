#ifndef VIEW_HEX_DUMP_H
#define VIEW_HEX_DUMP_H

#include "View.h"
#include "Disassembler.h"

#include <wx/htmllbox.h>
#include <vector>

class ViewHexDump : public View
{
public:
	ViewHexDump(ElfFile *file, wxString location);

protected:
	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);
	virtual void doSetOffset(int offset);

	int mSection;
	int mBase;
	int mOffset;
	int mFileSize;
	int mSize;
	bool mRel;
	std::vector<int> mOffsets;
	Disassembler mDisassembler;

	wxSimpleHtmlListBox *mHtmlListBox;
};
#endif