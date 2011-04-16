#include "ViewDynamic.h"

#include "Location.h"

#include <wx/msgdlg.h>
#include <wx/dcclient.h>

ViewDynamic::ViewDynamic(ElfFile *file, wxString location)
: View(file, location)
{
	if(Location::GetSectionString(location, 0) == "section") {
		int section = Location::GetSectionInt(location, 1);
		const Elf32_Shdr *header = GetFile()->GetSectionHeader(section);
		mOffset = header->sh_offset;
		mSize = header->sh_size;
		SetName(GetFile()->GetSectionName(section));
	} else if(Location::GetSectionString(location, 0) == "segment") {
		int segment = Location::GetSectionInt(location, 1);
		const Elf32_Phdr *header = GetFile()->GetProgramHeader(segment);
		mOffset = header->p_offset;
		mSize = header->p_filesz;
		SetName(wxString::Format("Program Header %i", segment));
	}
}

static wxString GetTagDescription(int tag)
{
	switch(tag) {
		case DT_NEEDED:
			return "NEEDED";
		case DT_PLTRELSZ:
			return "PLTRELSZ";
		case DT_PLTGOT:
			return "PLTGOT";
		case DT_HASH:
			return "HASH";
		case DT_STRTAB:
			return "STRTAB";
		case DT_SYMTAB:
			return "SYMTAB";
		case DT_RELA:
			return "RELA";
		case DT_RELASZ:
			return "RELASZ";
		case DT_RELAENT:
			return "RELAENT";
		case DT_STRSZ:
			return "STRSZ";
		case DT_SYMENT:
			return "SYMENT";
		case DT_INIT:
			return "INIT";
		case DT_FINI:
			return "FINI";
		case DT_SONAME:
			return "SONAME";
		case DT_RPATH:
			return "RPATH";
		case DT_SYMBOLIC:
			return "SYMBOLIC";
		case DT_REL:
			return "REL";
		case DT_RELSZ:
			return "RELSZ";
		case DT_RELENT:
			return "RELENT";
		case DT_PLTREL:
			return "PLTREL";
		case DT_DEBUG:
			return "DEBUG";
		case DT_TEXTREL:
			return "TEXTREL";
		case DT_JMPREL:
			return "JMPREL";
		default:
			return "(None)";
	}
}

wxWindow *ViewDynamic::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mTable = new LinkTable(parent, id);

	int numEntries = mSize / sizeof(Elf32_Dyn);

	mTable->Setup(numEntries, 2);
	mTable->SetColumnLabel(0, "Tag");
	mTable->SetColumnLabel(1, "Value");
	
	char *buffer = new char[mSize];
	GetFile()->Read(buffer, mOffset, mSize);

	int strtab = GetFile()->GetDynamicEntry(DT_STRTAB)->d_un.d_ptr;
	int strsz = GetFile()->GetDynamicEntry(DT_STRSZ)->d_un.d_val;

	int phdr_num = GetFile()->GetContainingProgramHeader(strtab);
	const Elf32_Phdr *phdr = GetFile()->GetProgramHeader(phdr_num);
	int offset = phdr->p_offset + strtab - phdr->p_vaddr;
	char *stringTable = new char[strsz];
	GetFile()->Read(stringTable, offset, strsz);
	
	for(int i=0; i<numEntries; i++) {
		Elf32_Dyn *dyn = (Elf32_Dyn*)(buffer + i * sizeof(Elf32_Dyn));

		mTable->SetCell(i, 0, GetTagDescription(dyn->d_tag));

		wxString value;
		switch(dyn->d_tag) {
			case DT_NEEDED:
			case DT_SONAME:
				value = wxString(stringTable + dyn->d_un.d_val);
				break;
			default:
				value = wxString::Format("0x%x", dyn->d_un.d_val);
		}

		wxString target;
		switch(dyn->d_tag) {
			case DT_INIT:
			case DT_FINI:
			case DT_HASH:
			case DT_STRTAB:
			case DT_SYMTAB:
			case DT_PLTGOT:
			case DT_JMPREL:
				target = Location::BuildLocation(GetFile()->GetToken(), "absolute", dyn->d_un.d_val);
				break;
			default:
				break;
		}
		mTable->SetCell(i, 1, value, target);
	}

	delete[] buffer;
	delete[] stringTable;

	return mTable;
}

void ViewDynamic::doSetOffset(int offset)
{
}