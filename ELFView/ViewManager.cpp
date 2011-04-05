#include "ViewManager.h"

#include "WindowMain.h"

#include "ViewElfHeader.h"
#include "ViewSectionHeaders.h"
#include "ViewProgramHeaders.h"
#include "ViewSymbolTable.h"

ViewManager::ViewManager(WindowMain *windowMain)
{
	mWindowMain = windowMain;
}

void ViewManager::GoToLocation(ElfFile *file, wxString location)
{
	int idx = AddLocation(file, location);	

	if(idx != -1) {
		mWindowMain->SwitchToViewPage(idx);
	}
}

int ViewManager::AddLocation(ElfFile *file, wxString location)
{
	int idx;
	View *view = FindView(file, location, idx);

	if(!view) {
		view = CreateView(file, location);

		if(view) {
			idx = AddView(view);
		} else {
			idx = -1;
		}
	}

	return idx;
}

void ViewManager::CloseAllViews(ElfFile *file)
{
	for(int i=0; i<mViewList.size(); i++) {
		View *view = mViewList[i];
		if(view->GetFile() == file) {
			mViewList.erase(mViewList.begin() + i);
			delete view;
			mWindowMain->RemoveViewPage(i);
			i--;
		}
	}
}

View *ViewManager::FindView(ElfFile *file, wxString location, int &idx)
{
	View *view = NULL;
	for(int i=0; i<mViewList.size(); i++) {
		View *view = mViewList[i];
		if(view->GetFile() == file && view->GetLocation() == location) {
			idx = i;
			return view;
		}
	}

	idx = -1;
	return NULL;
}

int ViewManager::AddView(View *view)
{
	if(view) {
		mViewList.push_back(view);

		wxWindow *window = view->CreateWindow(mWindowMain, wxID_ANY);
		mWindowMain->AddViewPage(window, view->GetName());

		return mViewList.size() - 1;
	}

	return -1;
}

View *ViewManager::CreateView(ElfFile *file, wxString location)
{
	if(location == "header") {
		return new ViewElfHeader(file, location);
	} else if(location == "section/headers") {
		return new ViewSectionHeaders(file, location);
	} else if(location == "segment/headers") {
		return new ViewProgramHeaders(file, location);
	} else if(location.StartsWith("section/")) {
		return new ViewSymbolTable(file, location); 
	} else return NULL;
}