#include "ViewRelocations.h"

#include "Location.h"

ViewRelocations::ViewRelocations(ElfFile *file, wxString location)
: View(file, location)
{
	mSection = Location::GetSectionInt(location, 1);
	SetName(GetFile()->GetSectionName(mSection));
}

wxWindow *ViewRelocations::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mTable = new LinkTable(parent, id);

	const Elf32_Shdr *header = GetFile()->GetSectionHeader(mSection);
	char *buffer = new char[header->sh_size];
	GetFile()->Read(buffer, header->sh_offset, header->sh_size);
	int numEntries = header->sh_size / header->sh_entsize;

	if(header->sh_type == SHT_RELA) {
		mTable->Setup(numEntries, 4);
	} else {
		mTable->Setup(numEntries, 3);
	}

	mTable->SetColumnLabel(0, "Offset");
	mTable->SetColumnLabel(1, "Symbol");
	mTable->SetColumnLabel(2, "Type");

	if(header->sh_type == SHT_RELA) {
		mTable->SetColumnLabel(3, "Addend");
	}

	for(int i=0; i<header->sh_size / header->sh_entsize; i++) {
		Elf32_Rela *rela = (Elf32_Rela*)(buffer + i * header->sh_entsize);

		mTable->SetCell(i, 0, wxString::Format("0x%x", rela->r_offset));
		int symbol = ELF32_R_SYM(rela->r_info);
		int symbolSection = header->sh_link;
		wxString target = Location::BuildElfLocation(GetFile(), wxString::Format("section/%i", symbolSection), symbol * GetFile()->GetSectionHeader(symbolSection)->sh_entsize);
		mTable->SetCell(i, 1, wxString::Format("%s", GetFile()->GetSymbolName(symbolSection, symbol).c_str()), target);
		mTable->SetCell(i, 2, wxString::Format("0x%x", ELF32_R_TYPE(rela->r_info)));

		if(header->sh_type == SHT_RELA) {
			mTable->SetCell(i, 3, wxString::Format("0x%x", rela->r_addend));
		}
	}

	mTable->AutoSizeColumns();

	delete[] buffer;

	return mTable;
}
		
void ViewRelocations::doSetOffset(int offset)
{
	int row = offset / GetFile()->GetSectionHeader(mSection)->sh_entsize;
	mTable->SelectRow(row);

}