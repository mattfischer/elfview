#include "WindowMain.h"

wxWindow *gWindowMain;

WindowMain::WindowMain(wxWindow *parent, wxWindowID id, ViewManager *viewManager)
: wxNotebook(parent, id)
{
	mViewManager = viewManager;

	mViewManager->Connect(EVT_VM_VIEW_ADDED, (wxObjectEventFunction)&WindowMain::OnViewAdded, NULL, this);
	mViewManager->Connect(EVT_VM_VIEW_REMOVED, (wxObjectEventFunction)&WindowMain::OnViewRemoved, NULL, this);
	mViewManager->Connect(EVT_VM_CURRENT_VIEW_CHANGED, (wxObjectEventFunction)&WindowMain::OnCurrentViewChanged, NULL, this);

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

	Show(true);
}

void WindowMain::OnViewAdded(wxCommandEvent &e)
{
	View *view = (View*)e.GetClientData();

	wxWindow *window = view->CreateWindow(this, wxID_ANY);
	AddPage(window, view->GetName());
}

void WindowMain::OnViewRemoved(wxCommandEvent &e)
{
	DeletePage(e.GetInt());
}

void WindowMain::OnCurrentViewChanged(wxCommandEvent &e)
{
	ChangeSelection(e.GetInt());
}