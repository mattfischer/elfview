#include "ViewElfHeader.h"

#include "Location.h"
#include "FlagManager.h"

#include <wx/htmllbox.h>

extern wxWindow *gWindowMain;
ViewElfHeader::ViewElfHeader(ElfFile *file, wxString location)
: View(file, location)
{
	SetName("ELF Header");
}

wxWindow *ViewElfHeader::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mTable = new LinkTable(parent, id);

	mTable->Setup(14, 2);
	
	const Elf32_Ehdr *header = GetFile()->GetHeader();

	wxString target;

	mTable->SetColumnLabel(0, "Name");
	mTable->SetColumnLabel(1, "Value");

	mTable->SetCell(0, 0, "Type");
	mTable->SetCell(0, 1, FlagManager::GetDescription(FlagManager::SetElfType, header->e_type),
		Location::BuildFlagLocation(FlagManager::SetElfType, header->e_type));

	mTable->SetCell(1, 0, "Machine");
	mTable->SetCell(1, 1, wxString::Format("0x%x", header->e_machine));

	mTable->SetCell(2, 0, "Version");
	mTable->SetCell(2, 1, wxString::Format("0x%x", header->e_version));

	if(header->e_entry != 0) {
		target = Location::BuildElfLocation(GetFile(), "absolute", header->e_entry);
	} else {
		target = "";
	}
	mTable->SetCell(3, 0, "Entry point");
	mTable->SetCell(3, 1, wxString::Format("0x%08x", header->e_entry), target);

	if(header->e_phoff != 0) {
		target = Location::BuildElfLocation(GetFile(), "segment/headers");
	} else {
		target = "";
	}
	mTable->SetCell(4, 0, "Program Header Offset");
	mTable->SetCell(4, 1, wxString::Format("0x%x", header->e_phoff), target);

	if(header->e_shoff != 0) {
		target = Location::BuildElfLocation(GetFile(), "section/headers");
	} else {
		target = "";
	}
	mTable->SetCell(5, 0, "Section Header Offset");
	mTable->SetCell(5, 1, wxString::Format("0x%x", header->e_shoff), target);

	mTable->SetCell(6, 0, "Flags");
	mTable->SetCell(6, 1, wxString::Format("0x%x", header->e_flags));

	mTable->SetCell(7, 0, "Header Size");
	mTable->SetCell(7, 1, wxString::Format("0x%x", header->e_ehsize));

	mTable->SetCell(8, 0, "Program Header Entry Size");
	mTable->SetCell(8, 1, wxString::Format("0x%x", header->e_phentsize));

	mTable->SetCell(9, 0, "Number of Program Headers");
	mTable->SetCell(9, 1, wxString::Format("0x%x", header->e_phnum));

	mTable->SetCell(10, 0, "Section Header Entry Size");
	mTable->SetCell(10, 1, wxString::Format("0x%x", header->e_shentsize));

	mTable->SetCell(11, 0, "Number of Section Headers");
	mTable->SetCell(11, 1, wxString::Format("0x%x", header->e_shnum));

	mTable->SetCell(12, 0, "Program Header Entry Size");
	mTable->SetCell(12, 1, wxString::Format("0x%x", header->e_phentsize));

	if(header->e_shstrndx != 0) {
		target = Location::BuildElfLocation(GetFile(), wxString::Format("section/%i", header->e_shstrndx));
	} else {
		target = "";
	}
	mTable->SetCell(13, 0, "Section Header String Table");
	mTable->SetCell(13, 1, GetFile()->GetSectionName(header->e_shstrndx), target);

	mTable->AutoSizeColumns();

	return mTable;
}

void ViewElfHeader::doSetOffset(int offset)
{
}