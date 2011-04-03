#include "WindowMain.h"

WindowMain::WindowMain(wxWindow *parent, wxWindowID id)
: wxNotebook(parent, id)
{
	mElfHeader = new wxSimpleHtmlListBox(this, -1);
	mSectionHeaders = new wxSimpleHtmlListBox(this, -1);
	mProgramHeaders = new wxSimpleHtmlListBox(this, -1);

	AddPage(mElfHeader, "ELF Header");
	AddPage(mSectionHeaders, "Section Headers");
	AddPage(mProgramHeaders, "Program Headers");
}