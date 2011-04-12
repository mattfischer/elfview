#include "ViewRelocations.h"

#include "Util.h"

ViewRelocations::ViewRelocations(ElfFile *file, wxString location)
: View(file, location)
{
	mSection = Util::GetSectionNumber(location);
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
		mTable->Setup(numEntries, 4, GetFile());
	} else {
		mTable->Setup(numEntries, 3, GetFile());
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
		mTable->SetCell(i, 1, wxString::Format("%s", GetFile()->GetSymbolName(header->sh_link, ELF32_R_SYM(rela->r_info)).c_str()));
		mTable->SetCell(i, 2, wxString::Format("0x%x", ELF32_R_TYPE(rela->r_info)));

		if(header->sh_type == SHT_RELA) {
			mTable->SetCell(i, 3, wxString::Format("0x%x", rela->r_addend));
		}
	}

	delete[] buffer;

	return mTable;
}
		