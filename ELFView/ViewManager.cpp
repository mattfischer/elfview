#include "ViewManager.h"

#include "ViewElfHeader.h"
#include "ViewSectionHeaders.h"
#include "ViewProgramHeaders.h"
#include "ViewSymbolTable.h"

DEFINE_EVENT_TYPE(EVT_VM_VIEW_ADDED)
DEFINE_EVENT_TYPE(EVT_VM_VIEW_REMOVED)
DEFINE_EVENT_TYPE(EVT_VM_CURRENT_VIEW_CHANGED)

ViewManager::ViewManager()
{
}

void ViewManager::GoToLocation(ElfFile *file, wxString location)
{
	int idx = AddLocation(file, location);	

	if(idx != -1) {
		wxCommandEvent evt(EVT_VM_CURRENT_VIEW_CHANGED);
		evt.SetInt(idx);
		ProcessEvent(evt);
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
			wxCommandEvent evt(EVT_VM_VIEW_REMOVED);
			evt.SetInt(i);
			ProcessEvent(evt);
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

		wxCommandEvent evt(EVT_VM_VIEW_ADDED);
		evt.SetClientData(view);
		ProcessEvent(evt);

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