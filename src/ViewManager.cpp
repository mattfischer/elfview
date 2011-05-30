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

wxDEFINE_EVENT(EVT_VM_VIEW_ADDED, wxCommandEvent);
wxDEFINE_EVENT(EVT_VM_VIEW_REMOVED, wxCommandEvent);
wxDEFINE_EVENT(EVT_VM_CURRENT_VIEW_CHANGED, wxCommandEvent);
wxDEFINE_EVENT(EVT_VM_HISTORY_INDEX_CHANGED, wxCommandEvent);

ViewManager::ViewManager(FileManager *fileManager)
{
	mFileManager = fileManager;
	mCurrentView = -1;
	mHistoryIndex = -1;
}

void ViewManager::GoToLocation(wxString location, bool addToHistory)
{
	int idx;

	location = TranslateLocation(location);

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
		if(addToHistory) {
			mHistoryIndex++;
			mHistory.resize(mHistoryIndex + 1);
			mHistory[mHistoryIndex] = location;
		}

		wxCommandEvent evt1(EVT_VM_CURRENT_VIEW_CHANGED);
		evt1.SetInt(idx);
		ProcessEvent(evt1);

		wxCommandEvent evt2(EVT_VM_HISTORY_INDEX_CHANGED);
		evt2.SetInt(mHistoryIndex);
		ProcessEvent(evt2);

		mCurrentView = idx;
		view->SetOffset(Location::GetOffset(location));
	}
}

void ViewManager::GoBackInHistory()
{
	if(mHistoryIndex > 0) {
		mHistoryIndex--;
		GoToLocation(mHistory[mHistoryIndex], false);
	}
}

void ViewManager::GoForwardInHistory()
{
	if(mHistoryIndex < mHistory.size() - 1) {
		mHistoryIndex++;
		GoToLocation(mHistory[mHistoryIndex], false);
	}
}

int ViewManager::GetHistoryIndex()
{
	return mHistoryIndex;
}

int ViewManager::GetHistoryCount()
{
	return mHistory.size();
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
	int token = Location::GetElfToken(location);
	ElfFile *file = mFileManager->FindFile(token);

	if(file == NULL) {
		return NULL;
	}

	wxString base = Location::GetBase(location);

	if(Location::GetSectionString(location, 1) == "header") {
		return new ViewElfHeader(file, base);
	} else if(Location::GetSectionString(location, 1) == "section") {
		if(Location::GetSectionString(location, 2) == "headers") {
			return new ViewSectionHeaders(file, base);
		} else {
			int section = Location::GetSectionInt(location, 2);
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
	} else if(Location::GetSectionString(location, 1) == "segment") {
		if(Location::GetSectionString(location, 2) ==  "headers") {
			return new ViewProgramHeaders(file, base);
		} else {
			int segment = Location::GetSectionInt(location, 2);
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

wxString ViewManager::TranslateLocation(wxString location)
{
	int token = Location::GetElfToken(location);
	ElfFile *file = mFileManager->FindFile(token);

	if(file == NULL) {
		return location;
	}

	if(Location::GetSectionString(location, 1) == "absolute") {
		int offset = Location::GetOffset(location);
		for(int i=0; i<file->GetHeader()->e_shnum; i++) {
			const Elf32_Shdr *header = file->GetSectionHeader(i);

			if(offset >= header->sh_addr && offset < header->sh_addr + header->sh_size) {
				wxString newLocation = Location::BuildElfLocation(file, wxString::Format("section/%i", i), offset - header->sh_addr);
				return newLocation;
			}
		}

		for(int i=0; i<file->GetHeader()->e_phnum; i++) {
			const Elf32_Phdr *header = file->GetProgramHeader(i);

			if(header->p_type == PT_LOAD && offset >= header->p_vaddr && offset < header->p_vaddr + header->p_memsz) {
				wxString newLocation = Location::BuildElfLocation(file, wxString::Format("segment/%i", i), offset - header->p_vaddr);
				return newLocation;
			}
		}
	}

	return location;
}