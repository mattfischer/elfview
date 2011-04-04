#include "WindowMain.h"

WindowMain::WindowMain(wxWindow *parent, wxWindowID id)
: wxNotebook(parent, id)
{
	mViewManager = new ViewManager(this);

	mViewManager->AddLocation("header");
	mViewManager->AddLocation("section/headers");
	mViewManager->AddLocation("segment/headers");
}

void WindowMain::SetFile(ElfFile *file)
{
	mViewManager->SetFile(file);
}

void WindowMain::AddWindow(wxWindow *window, wxString name)
{
	AddPage(window, name);
}

void WindowMain::SwitchToWindow(int window)
{
	ChangeSelection(window);
}