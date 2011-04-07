#include "WindowNavigator.h"

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

void WindowNavigator::SetFile(ElfFile *file)
{
	mFile = file;
	
	wxArrayString arrayString;
	mSectionList->Clear();

	arrayString.Add("View Section Headers");
	for(int i=0;i<mFile->GetHeader()->e_shnum;i++) {
		wxString name = mFile->GetSectionName(i);

		if(name == "") {
			arrayString.Add(wxString::Format("%i", i));
		} else {
			arrayString.Add(wxString::Format("%i (%s)", i, name.c_str()));
		}
	}

	mSectionList->Append(arrayString);

	mSegmentList->Clear();
	arrayString.Clear();

	arrayString.Add("View Program Headers");
	for(int i=0; i<mFile->GetHeader()->e_phnum;i++) {
		arrayString.Add(wxString::Format("%i", i));
	}

	mSegmentList->Append(arrayString);
}

void WindowNavigator::OnSectionSelected(wxCommandEvent &e)
{
	int idx = e.GetInt();

	if(idx == 0) {
		mViewManager->GoToLocation(mFile, "section/headers");
	} else {
		mViewManager->GoToLocation(mFile, wxString::Format("section/%i", idx - 1));
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