#ifndef VIEW_PROGRAM_HEADERS_H
#define VIEW_PROGRAM_HEADERS_H

#include <wx/htmllbox.h>

#include "ElfFile.h"

class ViewProgramHeaders : public wxSimpleHtmlListBox
{
public:
	ViewProgramHeaders(wxWindow *parent, wxWindowID id);

	void SetFile(ElfFile *file);

protected:
	ElfFile *mFile;
};
#endif