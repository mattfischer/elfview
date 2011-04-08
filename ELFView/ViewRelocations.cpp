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
	mListCtrl = new wxListCtrl(parent, id);

	mListCtrl->SetSingleStyle(wxLC_REPORT);
	const Elf32_Shdr *header = GetFile()->GetSectionHeader(mSection);

	char *buffer = new char[header->sh_size];

	GetFile()->Read(buffer, header->sh_offset, header->sh_size);

	mListCtrl->InsertColumn(0, "Offset");
	mListCtrl->InsertColumn(1, "Info");

	if(header->sh_type == SHT_RELA) {
		mListCtrl->InsertColumn(2, "Addend");
	}

	for(int i=0; i<header->sh_size / header->sh_entsize; i++) {
		Elf32_Rela *rela = (Elf32_Rela*)(buffer + i * header->sh_entsize);

		mListCtrl->InsertItem(i, "");
		mListCtrl->SetItem(i, 0, wxString::Format("0x%x", rela->r_offset));
		mListCtrl->SetItem(i, 1, wxString::Format("0x%x", rela->r_info));
		if(header->sh_type == SHT_RELA) {
			mListCtrl->SetItem(i, 2, wxString::Format("0x%x", rela->r_addend));
		}
	}

	delete[] buffer;

	return mListCtrl;
}
		