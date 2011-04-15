#include "ViewSymbolTable.h"

#include "Location.h"

ViewSymbolTable::ViewSymbolTable(ElfFile *file, wxString location)
: View(file, location)
{
	mSection = Location::GetSectionInt(location, 1);
	SetName(GetFile()->GetSectionName(mSection));
}

static wxString GetBindDescription(int bind)
{
	switch(bind) {
		case STB_LOCAL:
			return "LOCAL";
		case STB_GLOBAL:
			return "GLOBAL";
		case STB_WEAK:
			return "WEAK";
		default:
			return "(Unknown)";
	}
}

static wxString GetTypeDescription(int type)
{
	switch(type) {
		case STT_NOTYPE:
			return "NONE";
		case STT_OBJECT:
			return "OBJECT";
		case STT_FUNC:
			return "FUNCTION";
		case STT_SECTION:
			return "SECTION";
		case STT_FILE:
			return "FILE";
		default:
			return "(Unknown)";
	}
}

static wxString GetSectionDescription(int section, ElfFile *file)
{
	switch(section) {
		case SHN_UNDEF:
			return "(Undefined)";
		case SHN_ABS:
			return "(Absolute)";
		case SHN_COMMON:
			return "(Common)";
		default:
			return file->GetSectionName(section);
	}
}

wxWindow *ViewSymbolTable::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mTable = new LinkTable(parent, id);

	const Elf32_Shdr *header = GetFile()->GetSectionHeader(mSection);
	char *buffer = new char[header->sh_size];
	GetFile()->Read(buffer, header->sh_offset, header->sh_size);
	int numEntries = header->sh_size / header->sh_entsize;

	mTable->Setup(numEntries, 6);

	mTable->SetColumnLabel(0, "Name");
	mTable->SetColumnLabel(1, "Value");
	mTable->SetColumnLabel(2, "Size");
	mTable->SetColumnLabel(3, "Bind");
	mTable->SetColumnLabel(4, "Type");
	mTable->SetColumnLabel(5, "Section");

	for(int i=1; i<header->sh_size / header->sh_entsize; i++) {
		Elf32_Sym *sym = (Elf32_Sym*)(buffer + i * header->sh_entsize);

		wxString target;

		target = Location::BuildLocation(GetFile()->GetToken(), wxString::Format("section/%i", header->sh_link), sym->st_name);

		mTable->SetCell(i-1, 0, GetFile()->GetString(header->sh_link, sym->st_name), target);

		if(sym->st_shndx != SHN_UNDEF && sym->st_shndx != SHN_ABS) {
			target = Location::BuildLocation(GetFile()->GetToken(), wxString::Format("section/%i", sym->st_shndx), sym->st_value);
		} else {
			target = "";
		}

		mTable->SetCell(i-1, 1, wxString::Format("0x%x", sym->st_value), target);
		mTable->SetCell(i-1, 2, wxString::Format("0x%x", sym->st_size));
		mTable->SetCell(i-1, 3, wxString::Format("%s", GetBindDescription(ELF32_ST_BIND(sym->st_info)).c_str()));
		mTable->SetCell(i-1, 4, wxString::Format("%s", GetTypeDescription(ELF32_ST_TYPE(sym->st_info)).c_str()));
		
		if(sym->st_shndx != SHN_UNDEF && sym->st_shndx != SHN_ABS) {
			target = Location::BuildLocation(GetFile()->GetToken(), wxString::Format("section/%i", sym->st_shndx));
		} else {
			target = "";
		}

		mTable->SetCell(i-1, 5, wxString::Format("%s", GetSectionDescription(sym->st_shndx, GetFile()).c_str()), target);
	}

	delete[] buffer;

	return mTable;
}

void ViewSymbolTable::doSetOffset(int offset)
{
	mTable->SelectRow(offset - 1);
}