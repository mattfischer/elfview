#include "ViewManager.h"

#include "ViewElfHeader.h"
#include "ViewSectionHeaders.h"
#include "ViewProgramHeaders.h"
#include "ViewSymbolTable.h"
#include "ViewRelocations.h"
#include "ViewStringTable.h"
#include "ViewHexDump.h"
#include "ViewDynamic.h"

#include "Location.h"

DEFINE_EVENT_TYPE(EVT_VM_VIEW_ADDED)
DEFINE_EVENT_TYPE(EVT_VM_VIEW_REMOVED)
DEFINE_EVENT_TYPE(EVT_VM_CURRENT_VIEW_CHANGED)

ViewManager::ViewManager(FileManager *fileManager)
{
	mFileManager = fileManager;
	mCurrentView = -1;
}

void ViewManager::GoToLocation(wxString location)
{
	int idx;
	View *view = FindView(location, idx);

	if(!view) {
		view = CreateView(location);

		if(view) {
			idx = AddView(view);
		} else {
			idx = -1;
		}
	}

	if(idx != -1) {
		wxCommandEvent evt(EVT_VM_CURRENT_VIEW_CHANGED);
		evt.SetInt(idx);
		ProcessEvent(evt);
		mCurrentView = idx;
		view->SetOffset(Location::GetOffsetInt(location));
	}
}

void ViewManager::CloseView(int view)
{
	View *ptr = mViewList[view];
	mViewList.erase(mViewList.begin() + view);
	delete ptr;
	wxCommandEvent evt(EVT_VM_VIEW_REMOVED);
	evt.SetInt(view);
	ProcessEvent(evt);

	if(mCurrentView == mViewList.size()) {
		mCurrentView--;
	}
}

void ViewManager::CloseAllViews(ElfFile *file)
{
	for(int i=0; i<mViewList.size(); i++) {
		View *view = mViewList[i];
		if(view->GetFile() == file) {
			CloseView(i);
			i--;
		}
	}
}

View *ViewManager::FindView(wxString location, int &idx)
{
	View *view = NULL;
	wxString base = Location::GetBase(location);

	for(int i=0; i<mViewList.size(); i++) {
		View *view = mViewList[i];
		if(view->GetLocation() == base) {
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

View *ViewManager::CreateView(wxString location)
{
	int token = Location::GetToken(location);
	ElfFile *file = mFileManager->FindFile(token);

	if(file == NULL) {
		return NULL;
	}

	wxString base = Location::GetBase(location);

	if(Location::GetSectionString(location, 0) == "header") {
		return new ViewElfHeader(file, base);
	} else if(Location::GetSectionString(location, 0) == "section") {
		if(Location::GetSectionString(location, 1) == "headers") {
			return new ViewSectionHeaders(file, base);
		} else {
			int section = Location::GetSectionInt(location, 1);
			const Elf32_Shdr *header = file->GetSectionHeader(section);

			switch(header->sh_type) {
				case SHT_REL:
				case SHT_RELA:
					return new ViewRelocations(file, base);
				case SHT_SYMTAB:
				case SHT_DYNSYM:
					return new ViewSymbolTable(file, base); 
				case SHT_STRTAB:
					return new ViewStringTable(file, base);
				case SHT_DYNAMIC:
					return new ViewDynamic(file, base);
				default:
					return new ViewHexDump(file, base);
			}
		}
	} else if(Location::GetSectionString(location, 0) == "segment") {
		if(Location::GetSectionString(location, 1) ==  "headers") {
			return new ViewProgramHeaders(file, base);
		} else {
			int segment = Location::GetSectionInt(location, 1);
			const Elf32_Phdr *header = file->GetProgramHeader(segment);

			switch(header->p_type) {
				case PT_DYNAMIC:
					return new ViewDynamic(file, base);
				default:
					return new ViewHexDump(file, base);
			}
		}
	}
	
	return NULL;
}