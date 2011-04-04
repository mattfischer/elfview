#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <wx/notebook.h>
#include <wx/htmllbox.h>

#include "ElfFile.h"

#include "ViewElfHeader.h"
#include "ViewSectionHeaders.h"
#include "ViewProgramHeaders.h"

class WindowMain : public wxNotebook
{
public:
	WindowMain(wxWindow *parent, wxWindowID id);

	void SetFile(ElfFile *file);

protected:
	ViewElfHeader *mElfHeader;
	ViewSectionHeaders *mSectionHeaders;
	ViewProgramHeaders *mProgramHeaders;

	ElfFile *mFile;
};

#endif