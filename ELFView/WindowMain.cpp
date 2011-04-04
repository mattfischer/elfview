#include "WindowMain.h"

#include "ViewElfHeader.h"
#include "ViewSectionHeaders.h"
#include "ViewProgramHeaders.h"

WindowMain::WindowMain(wxWindow *parent, wxWindowID id)
: wxNotebook(parent, id)
{
	mFile = NULL;

	AddView(new ViewElfHeader());
	AddView(new ViewSectionHeaders());
	AddView(new ViewProgramHeaders());
}

void WindowMain::SetFile(ElfFile *file)
{
	mFile = file;

	for(ViewList::iterator it = mViewList.begin(); it != mViewList.end(); it++) {
		(*it)->SetFile(file);
	}
}

void WindowMain::AddView(View *view)
{
	mViewList.Append(view);
	view->SetFile(mFile);

	wxWindow *window = view->CreateWindow(this, -1);
	AddPage(window, view->GetName());
}