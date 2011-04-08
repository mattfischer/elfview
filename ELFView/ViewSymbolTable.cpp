#include "ViewSymbolTable.h"

#include "Util.h"

ViewSymbolTable::ViewSymbolTable(ElfFile *file, wxString location)
: View(file, location)
{
	mSection = Util::GetSectionNumber(location);
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
	mListCtrl->InsertColumn(3, "Bind");
	mListCtrl->InsertColumn(4, "Type");
	mListCtrl->InsertColumn(5, "Section");

	for(int i=1; i<header->sh_size / header->sh_entsize; i++) {
		Elf32_Sym *sym = (Elf32_Sym*)(buffer + i * header->sh_entsize);

		mListCtrl->InsertItem(i-1, "");
		mListCtrl->SetItem(i-1, 0, GetFile()->GetString(header->sh_link, sym->st_name));
		mListCtrl->SetItem(i-1, 1, wxString::Format("0x%x", sym->st_value));
		mListCtrl->SetItem(i-1, 2, wxString::Format("0x%x", sym->st_size));
		mListCtrl->SetItem(i-1, 3, wxString::Format("%s", GetBindDescription(ELF32_ST_BIND(sym->st_info)).c_str()));
		mListCtrl->SetItem(i-1, 4, wxString::Format("%s", GetTypeDescription(ELF32_ST_TYPE(sym->st_info)).c_str()));
		mListCtrl->SetItem(i-1, 5, wxString::Format("%s", GetSectionDescription(sym->st_shndx).c_str()));
	}

	delete[] buffer;

	return mListCtrl;
}

wxString ViewSymbolTable::GetBindDescription(int bind)
{
	switch(bind) {
		case STB_LOCAL:
			return "Local";
		case STB_GLOBAL:
			return "Global";
		case STB_WEAK:
			return "Weak";
		default:
			return "(Unknown)";
	}
}

wxString ViewSymbolTable::GetTypeDescription(int type)
{
	switch(type) {
		case STT_NOTYPE:
			return "None";
		case STT_OBJECT:
			return "Object";
		case STT_FUNC:
			return "Function";
		case STT_SECTION:
			return "Section";
		case STT_FILE:
			return "File";
		default:
			return "(Unknown)";
	}
}

wxString ViewSymbolTable::GetSectionDescription(int section)
{
	switch(section) {
		case SHN_UNDEF:
			return "(Undefined)";
		case SHN_ABS:
			return "(Absolute)";
		case SHN_COMMON:
			return "(Common)";
		default:
			return Util::GetSectionTitle(GetFile(), section);
	}
}