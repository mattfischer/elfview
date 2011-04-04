#include "WindowMain.h"

WindowMain::WindowMain(wxWindow *parent, wxWindowID id)
: wxNotebook(parent, id)
{
	mElfHeader = new ViewElfHeader(this, -1);
	mSectionHeaders = new ViewSectionHeaders(this, -1);
	mProgramHeaders = new ViewProgramHeaders(this, -1);

	AddPage(mElfHeader, "ELF Header");
	AddPage(mSectionHeaders, "Section Headers");
	AddPage(mProgramHeaders, "Program Headers");
}

void WindowMain::SetFile(ElfFile *file)
{
	mFile = file;

	mElfHeader->SetFile(file);
	mSectionHeaders->SetFile(file);
	mProgramHeaders->SetFile(file);
}