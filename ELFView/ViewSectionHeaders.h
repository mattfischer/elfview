#ifndef VIEW_SECTION_HEADERS_H
#define VIEW_SECTION_HEADERS_H

#include <wx/htmllbox.h>

#include "ElfFile.h"

class ViewSectionHeaders : public wxSimpleHtmlListBox
{
public:
	ViewSectionHeaders(wxWindow *parent, wxWindowID id);

	void SetFile(ElfFile *file);

protected:
	ElfFile *mFile;
};
#endif