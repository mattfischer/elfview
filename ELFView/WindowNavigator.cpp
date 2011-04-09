#include "WindowNavigator.h"

#include "Util.h"

enum {
	ID_Section_List,
	ID_Segment_List
};

WindowNavigator::WindowNavigator(wxWindow *parent, wxWindowID id, ViewManager *viewManager)
: wxNotebook(parent, id)
{
	mSectionList = new wxListBox(this, ID_Section_List);
	mSegmentList = new wxListBox(this, ID_Segment_List);
	mViewManager = viewManager;

	AddPage(mSectionList, "Sections");
	AddPage(mSegmentList, "Segments");
}

static wxString GetPhdrTypeDescription(int type)
{
	switch(type) {
		case PT_LOAD:
			return "LOAD";
		case PT_DYNAMIC:
			return "DYNAMIC";
		case PT_INTERP:
			return "INTERP";
		case PT_NOTE:
			return "NOTE";
		case PT_SHLIB:
			return "SHLIB";
		case PT_PHDR:
			return "PHDR";
		default:
			return "";
	}
}

void WindowNavigator::SetFile(ElfFile *file)
{
	mFile = file;
	
	wxArrayString arrayString;
	mSectionList->Clear();

	arrayString.Add("View Section Headers");
	for(int i=1;i<mFile->GetHeader()->e_shnum;i++) {
		arrayString.Add(wxString::Format("%s", Util::GetSectionTitle(mFile, i).c_str()));
	}

	mSectionList->Append(arrayString);

	mSegmentList->Clear();
	arrayString.Clear();

	arrayString.Add("View Program Headers");
	for(int i=0; i<mFile->GetHeader()->e_phnum;i++) {
		const Elf32_Phdr *header = mFile->GetProgramHeader(i);
		wxString desc = GetPhdrTypeDescription(header->p_type);

		if(desc == "") {
			arrayString.Add(wxString::Format("%i", i));
		} else {
			arrayString.Add(wxString::Format("%i (%s)", i, desc.c_str()));
		}
	}

	mSegmentList->Append(arrayString);
}

void WindowNavigator::OnSectionSelected(wxCommandEvent &e)
{
	int idx = e.GetInt();

	if(idx == 0) {
		mViewManager->GoToLocation(mFile, "section/headers");
	} else {
		mViewManager->GoToLocation(mFile, wxString::Format("section/%i", idx));
	}
}

void WindowNavigator::OnSegmentSelected(wxCommandEvent &e)
{
	int idx = e.GetInt();

	if(idx == 0) {
		mViewManager->GoToLocation(mFile, "segment/headers");
	} else {
		mViewManager->GoToLocation(mFile, wxString::Format("segment/%i", idx - 1));
	}
}

BEGIN_EVENT_TABLE(WindowNavigator, wxNotebook)
	EVT_LISTBOX_DCLICK(ID_Section_List, WindowNavigator::OnSectionSelected)
	EVT_LISTBOX_DCLICK(ID_Segment_List, WindowNavigator::OnSegmentSelected)
END_EVENT_TABLE()