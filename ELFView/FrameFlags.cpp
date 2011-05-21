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

	wxString section = Location::GetSectionString(location, 0);
	int offset = Location::GetOffsetInt(location);

	mGrid->DeleteRows(0, mValues.size());
	mValues.clear();

	SetupValues(section);

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

void FrameFlags::SetupValues(wxString section)
{
	if(section == "elf-type") {
		AddValue("NONE", ET_NONE);
		AddValue("REL",  ET_REL);
		AddValue("EXEC", ET_EXEC);
		AddValue("DYN",  ET_DYN);
		AddValue("CORE", ET_CORE);
	} else if(section == "section-type") {
		AddValue("NULL",     SHT_NULL);
		AddValue("PROGBITS", SHT_PROGBITS);
		AddValue("SYMTAB",   SHT_SYMTAB);
		AddValue("STRTAB",   SHT_STRTAB);
		AddValue("RELA",     SHT_RELA);
		AddValue("HASH",     SHT_HASH);
		AddValue("DYNAMIC",  SHT_DYNAMIC);
		AddValue("NOTE",     SHT_NOTE);
		AddValue("NOBITS",   SHT_NOBITS);
		AddValue("REL",      SHT_REL);
		AddValue("SHLIB",    SHT_SHLIB);
		AddValue("DYNSYM",   SHT_DYNSYM);
	} else if(section == "symbol-bind") {
		AddValue("LOCAL",  STB_LOCAL);
		AddValue("GLOBAL", STB_GLOBAL);
		AddValue("WEAK",   STB_WEAK);
	} else if(section == "symbol-type") {
		AddValue("NOTYPE",  STT_NOTYPE);
		AddValue("OBJECT",  STT_OBJECT);
		AddValue("FUNC",    STT_FUNC);
		AddValue("SECTION", STT_SECTION);
		AddValue("FILE",    STT_FILE);
	} else if(section == "segment-type") {
		AddValue("NULL",    PT_NULL);
		AddValue("LOAD",    PT_LOAD);
		AddValue("DYNAMIC", PT_DYNAMIC);
		AddValue("INTERP",  PT_INTERP);
		AddValue("NOTE",    PT_NOTE);
		AddValue("SHLIB",   PT_SHLIB);
		AddValue("PHDR",    PT_PHDR);
	} else if(section == "dynamic-type") {
		AddValue("NULL",     DT_NULL);
		AddValue("NEEDED",   DT_NEEDED);
		AddValue("PLTRELSZ", DT_PLTRELSZ);
		AddValue("PLTGOT",   DT_PLTGOT);
		AddValue("HASH",     DT_HASH);
		AddValue("STRTAB",   DT_STRTAB);
		AddValue("SYMTAB",   DT_SYMTAB);
		AddValue("RELA",     DT_RELA);
		AddValue("RELASZ",   DT_RELASZ);
		AddValue("RELAENT",  DT_RELAENT);
		AddValue("STRSZ",    DT_STRSZ);
		AddValue("SYMENT",   DT_SYMENT);
		AddValue("INIT",     DT_INIT);
		AddValue("FINI",     DT_FINI);
		AddValue("SONAME",   DT_SONAME);
		AddValue("RPATH",    DT_RPATH);
		AddValue("SYMBOLIC", DT_SYMBOLIC);
		AddValue("REL",      DT_REL);
		AddValue("RELSZ",    DT_RELSZ);
		AddValue("RELENT",   DT_RELENT);
		AddValue("PLTREL",   DT_PLTREL);
		AddValue("DEBUG",    DT_DEBUG);
		AddValue("TEXTREL",  DT_TEXTREL);
		AddValue("JMPREL",   DT_JMPREL);
	}
}

void FrameFlags::AddValue(wxString name, int value)
{
	mValues.push_back(FlagValue(name, value));
}

BEGIN_EVENT_TABLE(FrameFlags, wxFrame)
	EVT_BUTTON(wxID_CLOSE, FrameFlags::OnClose)
END_EVENT_TABLE()