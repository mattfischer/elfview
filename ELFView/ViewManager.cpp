#include "ViewManager.h"

#include "WindowMain.h"

#include "ViewElfHeader.h"
#include "ViewSectionHeaders.h"
#include "ViewProgramHeaders.h"

ViewManager::ViewManager(WindowMain *windowMain)
{
	mWindowMain = windowMain;
	mFile = NULL;
}

void ViewManager::SetFile(ElfFile *file)
{
	mFile = file;

	for(ViewList::iterator it = mViewList.begin(); it != mViewList.end(); it++) {
		(*it)->SetFile(file);
	}
}

View *ViewManager::FindView(wxString location, int &idx)
{
	View *view = NULL;
	int i = 0;
	for(ViewList::iterator it = mViewList.begin(); it != mViewList.end(); it++) {
		View *view = *it;
		if(view->GetLocation() == location) {
			idx = i;
			return view;
		}
		i++;
	}

	idx = -1;
	return NULL;
}

int ViewManager::AddView(View *view)
{
	if(view) {
		mViewList.Append(view);
		view->SetFile(mFile);

		wxWindow *window = view->CreateWindow(mWindowMain, -1);
		mWindowMain->AddWindow(window, view->GetName());

		return mViewList.GetCount() - 1;
	}

	return -1;
}

int ViewManager::AddLocation(wxString location)
{
	int idx;
	View *view = FindView(location, idx);

	if(!view) {
		view = CreateView(location);

		if(view) {
			view->SetLocation(location);
			idx = AddView(view);
		} else {
			idx = -1;
		}
	}

	return idx;
}

void ViewManager::GoToLocation(wxString location)
{
	int idx = AddLocation(location);	

	if(idx != -1) {
		mWindowMain->SwitchToWindow(idx);
	}
}

View *ViewManager::CreateView(wxString location)
{
	if(location == "header") {
		return new ViewElfHeader();
	} else if(location == "section/headers") {
		return new ViewSectionHeaders();
	} else if(location == "segment/headers") {
		return new ViewProgramHeaders();
	} else return NULL;
}