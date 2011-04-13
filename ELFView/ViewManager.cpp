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
	}
}

/*int ViewManager::AddLocation(ElfFile *file, wxString location)
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
*/

void ViewManager::CloseView(int view)
{
	View *ptr = mViewList[view];
	mViewList.erase(mViewList.begin() + view);
	delete ptr;
	wxCommandEvent evt(EVT_VM_VIEW_REMOVED);
	evt.SetInt(view);
	ProcessEvent(evt);
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
		if(view->GetLocation() == location) {
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

	if(Location::GetSectionString(location, 0) == "header") {
		return new ViewElfHeader(file, location);
	} else if(Location::GetSectionString(location, 0) == "section") {
		if(Location::GetSectionString(location, 1) == "headers") {
			return new ViewSectionHeaders(file, location);
		} else {
			int section = Location::GetSectionInt(location, 1);
			const Elf32_Shdr *header = file->GetSectionHeader(section);

			switch(header->sh_type) {
				case SHT_REL:
				case SHT_RELA:
					return new ViewRelocations(file, location);
				case SHT_SYMTAB:
				case SHT_DYNSYM:
					return new ViewSymbolTable(file, location); 
				case SHT_STRTAB:
					return new ViewStringTable(file, location);
				case SHT_DYNAMIC:
					return new ViewDynamic(file, location);
				default:
					return new ViewHexDump(file, location);
			}
		}
	} else if(Location::GetSectionString(location, 0) == "segment") {
		if(Location::GetSectionString(location, 1) ==  "headers") {
			return new ViewProgramHeaders(file, location);
		} else {
			int segment = Location::GetSectionInt(location, 1);
			const Elf32_Phdr *header = file->GetProgramHeader(segment);

			switch(header->p_type) {
				case PT_DYNAMIC:
					return new ViewDynamic(file, location);
				default:
					return new ViewHexDump(file, location);
			}
		}
	}
	
	return NULL;
}