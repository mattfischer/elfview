#ifndef VIEW_ELF_HEADER_H
#define VIEW_ELF_HEADER_H

#include <wx/htmllbox.h>

#include "ElfFile.h"

class ViewElfHeader : public wxSimpleHtmlListBox
{
public:
	ViewElfHeader(wxWindow *parent, wxWindowID id);

	void SetFile(ElfFile *file);

protected:
	ElfFile *mFile;
};
#endif