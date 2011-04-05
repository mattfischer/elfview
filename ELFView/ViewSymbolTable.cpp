#include "ViewSymbolTable.h"

ViewSymbolTable::ViewSymbolTable(ElfFile *file, wxString location)
: View(file, location)
{
	int idx;
	
	idx = location.Find('/', true);
	wxString number = location.SubString(idx + 1, location.size());
	number.ToLong(&mSection);

	SetName(GetFile()->GetSectionName(mSection));
}

wxWindow *ViewSymbolTable::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mListCtrl = new wxListCtrl(parent, id);

	mListCtrl->SetSingleStyle(wxLC_REPORT);
	const Elf32_Shdr *header = GetFile()->GetSectionHeader(mSection);

	char *buffer = new char[header->sh_size];

	GetFile()->Read(buffer, header->sh_offset, header->sh_size);

	mListCtrl->InsertColumn(0, "Name");
	mListCtrl->InsertColumn(1, "Value");
	mListCtrl->InsertColumn(2, "Size");
	mListCtrl->InsertColumn(3, "Info");
	mListCtrl->InsertColumn(4, "Other");
	mListCtrl->InsertColumn(5, "Section");

	for(int i=0; i<header->sh_size / header->sh_entsize; i++) {
		Elf32_Sym *sym = (Elf32_Sym*)(buffer + i * header->sh_entsize);

		mListCtrl->InsertItem(i, "");
		mListCtrl->SetItem(i, 0, GetFile()->GetString(header->sh_link, sym->st_name));
		mListCtrl->SetItem(i, 1, wxString::Format("0x%x", sym->st_value));
		mListCtrl->SetItem(i, 2, wxString::Format("0x%x", sym->st_size));
		mListCtrl->SetItem(i, 3, wxString::Format("0x%x", sym->st_info));
		mListCtrl->SetItem(i, 4, wxString::Format("0x%x", sym->st_other));
		mListCtrl->SetItem(i, 5, wxString::Format("0x%x", sym->st_shndx));
	}

	return mListCtrl;
}