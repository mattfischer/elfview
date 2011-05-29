#include "FrameFlags.h"

#include <wx/sizer.h>
#include <wx/button.h>

#include "Location.h"

static FrameFlags *gInstance = NULL;

FrameFlags *FrameFlags::GetInstance(wxWindow *parent, wxWindowID id)
{
	if(gInstance == NULL) {
		gInstance = new FrameFlags(parent, id);
	}

	return gInstance;
}

FrameFlags::FrameFlags(wxWindow *parent, wxWindowID id)
: wxFrame(parent, id, "Flags")
{
	mGrid = new wxGrid(this, wxID_ANY);
	mGrid->HideRowLabels();
	mGrid->UseNativeColHeader();
	mGrid->EnableGridLines(false);
	mGrid->SetCellHighlightPenWidth(0);
	mGrid->EnableEditing(false);
	mGrid->DisableDragRowSize();
	mGrid->CreateGrid(1, 2, wxGrid::wxGridSelectRows);
	mGrid->SetColLabelValue(0, "Name");
	mGrid->SetColLabelValue(1, "Value");
}

FrameFlags::~FrameFlags()
{
	gInstance = NULL;
}

void FrameFlags::OnClose(wxCommandEvent &e)
{
	Destroy();
}

void FrameFlags::SetLocation(wxString location)
{
	mLocation = location;

	int section = Location::GetSectionInt(location, 0);
	int offset = Location::GetOffset(location);

	mGrid->DeleteRows(0, mValues.size());
	mValues = FlagManager::GetValues((FlagManager::Set)section);

	mGrid->InsertRows(0, mValues.size());
	for(int i=0; i<mValues.size(); i++) {
		mGrid->SetCellValue(i, 0, mValues[i].name);
		mGrid->SetCellValue(i, 1, wxString::Format("0x%x", mValues[i].value));
	}
	mGrid->AutoSizeColumns();

	for(int i=0; i<mValues.size(); i++) {
		if(mValues[i].value == offset) {
			SetRowBold(i, true);
			mGrid->MakeCellVisible(i, 0);
		} else {
			SetRowBold(i, false);
		}
	}
}

void FrameFlags::SetRowBold(int row, bool bold)
{
	for(int col=0; col<2; col++) {
		wxFont font = mGrid->GetCellFont(row, col);
		font.SetWeight(bold ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
		mGrid->SetCellFont(row, col, font);
	}
}

BEGIN_EVENT_TABLE(FrameFlags, wxFrame)
	EVT_BUTTON(wxID_CLOSE, FrameFlags::OnClose)
END_EVENT_TABLE()