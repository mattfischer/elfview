#ifndef VIEW_HEX_DUMP_H
#define VIEW_HEX_DUMP_H

#include "View.h"

#include <wx/htmllbox.h>

class ViewHexDump : public View
{
public:
	ViewHexDump(ElfFile *file, wxString location);

protected:
	virtual wxWindow *doCreateWindow(wxWindow *parent, wxWindowID id);

	int mSection;
	int mBase;
	int mOffset;
	int mFileSize;
	int mSize;
	bool mRel;
	wxSimpleHtmlListBox *mHtmlListBox;
};
#endif