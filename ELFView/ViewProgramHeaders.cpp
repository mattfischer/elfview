#include "ViewProgramHeaders.h"

#include "Location.h"

extern wxWindow *gWindowMain;

ViewProgramHeaders::ViewProgramHeaders(ElfFile *file, wxString location)
: View(file, location)
{
	SetName("Program Headers");
}

static wxString GetTypeDescription(int type)
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

wxWindow *ViewProgramHeaders::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mTable = new LinkTable(parent, id);

	if(GetFile()->GetHeader()->e_phnum > 0) {
		mTable->Setup(10 * GetFile()->GetHeader()->e_phnum, 2);
		mTable->SetColumnLabel(0, "Name");
		mTable->SetColumnLabel(1, "Value");

		for(int i=0; i<GetFile()->GetHeader()->e_phnum;i++) {
			const Elf32_Phdr *header = GetFile()->GetProgramHeader(i);

			int rowStart = 10 * i;

			mTable->SetCell(rowStart, 0, wxString::Format("Segment %i", i));
			mTable->SetCellSize(rowStart, 0, 1, 2);
			wxFont font = mTable->GetCellFont(rowStart, 0);
			font.SetWeight(wxFONTWEIGHT_BOLD);
			mTable->SetCellFont(rowStart, 0, font);

			mTable->SetCell(rowStart + 1, 0, "Type");
			mTable->SetCell(rowStart + 1, 1, GetTypeDescription(header->p_type), Location::BuildLocation("flags", "segment-type", header->p_type));

			mTable->SetCell(rowStart + 2, 0, "Offset");
			wxString target = Location::BuildElfLocation(GetFile(), wxString::Format("segment/%i", i));
			mTable->SetCell(rowStart + 2, 1, wxString::Format("0x%x", header->p_offset), target);

			mTable->SetCell(rowStart + 3, 0, "VAddr");
			mTable->SetCell(rowStart + 3, 1, wxString::Format("0x%x", header->p_vaddr));

			mTable->SetCell(rowStart + 4, 0, "PAddr");
			mTable->SetCell(rowStart + 4, 1, wxString::Format("0x%x", header->p_paddr));

			mTable->SetCell(rowStart + 5, 0, "File Size");
			mTable->SetCell(rowStart + 5, 1, wxString::Format("0x%x", header->p_filesz));

			mTable->SetCell(rowStart + 6, 0, "Memory Size");
			mTable->SetCell(rowStart + 6, 1, wxString::Format("0x%x", header->p_memsz));

			mTable->SetCell(rowStart + 7, 0, "Flags");
			mTable->SetCell(rowStart + 7, 1, wxString::Format("0x%x", header->p_flags));

			mTable->SetCell(rowStart + 8, 0, "Alignment");
			mTable->SetCell(rowStart + 8, 1, wxString::Format("0x%x", header->p_align));
		}
	} else {
		mTable->Setup(1, 2);
		mTable->SetColumnLabel(0, "Name");
		mTable->SetColumnLabel(1, "Value");

		mTable->SetCell(0, 0, "No Program Headers");
		mTable->SetCellSize(0, 0, 1, 2);
		wxFont font = mTable->GetCellFont(0, 0);
		font.SetStyle(wxFONTSTYLE_ITALIC);
		mTable->SetCellFont(0, 0, font);
	}

	mTable->AutoSizeColumns();

	return mTable;
}

void ViewProgramHeaders::doSetOffset(int offset)
{
}