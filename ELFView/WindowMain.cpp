#include "WindowMain.h"

wxWindow *gWindowMain;

WindowMain::WindowMain(wxWindow *parent, wxWindowID id)
: wxNotebook(parent, id)
{
	mViewManager = new ViewManager(this);
	mFile = NULL;

	gWindowMain = this;
}

void WindowMain::SetFile(ElfFile *file)
{
	Show(false);
	if(mFile != NULL) {
		mViewManager->CloseAllViews(mFile);
		delete mFile;
	}
	mFile = file;
	
	mViewManager->AddLocation(file, "header");
	mViewManager->AddLocation(file, "section/headers");
	mViewManager->AddLocation(file, "segment/headers");
	mViewManager->AddLocation(file, "section/8");
	Show(true);
}

void WindowMain::AddViewPage(wxWindow *window, wxString name)
{
	AddPage(window, name);
}

void WindowMain::SwitchToViewPage(int page)
{
	ChangeSelection(page);
}

void WindowMain::RemoveViewPage(int page)
{
	DeletePage(page);
}