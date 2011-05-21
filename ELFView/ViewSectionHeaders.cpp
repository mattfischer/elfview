#include "ViewSectionHeaders.h"

#include "Location.h"

ViewSectionHeaders::ViewSectionHeaders(ElfFile *file, wxString location)
: View(file, location)
{
	SetName("Section Headers");
}

static wxString GetTypeDescription(int type)
{
	switch(type) {
		case SHT_NULL:
			return "NULL";
		case SHT_PROGBITS:
			return "PROGBITS";
		case SHT_SYMTAB:
			return "SYMTAB";
		case SHT_STRTAB:
			return "STRTAB";
		case SHT_RELA:
			return "RELA";
		case SHT_HASH:
			return "HASH";
		case SHT_DYNAMIC:
			return "DYNAMIC";
		case SHT_NOTE:
			return "NOTE";
		case SHT_NOBITS:
			return "NOBITS";
		case SHT_REL:
			return "REL";
		case SHT_SHLIB:
			return "SHLIB";
		case SHT_DYNSYM:
			return "DYNSYM";
		default:
			return "(Unknown)";
	}
}

wxWindow *ViewSectionHeaders::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mTable = new LinkTable(parent, id);

	if(GetFile()->GetHeader()->e_shnum > 0) {
		mTable->Setup(12 * (GetFile()->GetHeader()->e_shnum - 1), 2);
		mTable->SetColumnLabel(0, "Name");
		mTable->SetColumnLabel(1, "Value");

		for(int i=1; i<GetFile()->GetHeader()->e_shnum - 1;i++) {
			const Elf32_Shdr *header = GetFile()->GetSectionHeader(i);

			wxString name;
			wxString target;
			int rowStart = 12 * (i - 1);

			mTable->SetCell(rowStart, 0, wxString::Format("Section %i", i));
			mTable->SetCellSize(rowStart, 0, 1, 2);
			wxFont font = mTable->GetCellFont(rowStart, 0);
			font.SetWeight(wxFONTWEIGHT_BOLD);
			mTable->SetCellFont(rowStart, 0, font);

			mTable->SetCell(rowStart + 1, 0, "Name");
			mTable->SetCell(rowStart + 1, 1, GetFile()->GetSectionName(i));

			mTable->SetCell(rowStart + 2, 0, "Type");
			mTable->SetCell(rowStart + 2, 1, GetTypeDescription(header->sh_type), Location::BuildLocation("flags", "section-type", header->sh_type));

			mTable->SetCell(rowStart + 3, 0, "Flags");
			mTable->SetCell(rowStart + 3, 1, wxString::Format("0x%x", header->sh_flags));

			mTable->SetCell(rowStart + 4, 0, "Address");
			mTable->SetCell(rowStart + 4, 1, wxString::Format("0x%x", header->sh_addr));

			mTable->SetCell(rowStart + 5, 0, "Offset");
			target = Location::BuildElfLocation(GetFile(), wxString::Format("section/%i", i));
			mTable->SetCell(rowStart + 5, 1, wxString::Format("0x%x", header->sh_offset), target);

			mTable->SetCell(rowStart + 6, 0, "Size");
			mTable->SetCell(rowStart + 6, 1, wxString::Format("0x%x", header->sh_size));

			mTable->SetCell(rowStart + 7, 0, "Link");
			if(header->sh_link == 0) {
				target = "";
				name = wxString::Format("0x%x", header->sh_link);
			} else {
				target = Location::BuildElfLocation(GetFile(), wxString::Format("section/%i", header->sh_link));
				name = GetFile()->GetSectionName(header->sh_link);
			}
			mTable->SetCell(rowStart + 7, 1, name, target);

			if(header->sh_info == 0 || header->sh_type == SHT_SYMTAB || header->sh_type == SHT_DYNSYM) {
				target = "";
				name = wxString::Format("0x%x", header->sh_info);
			} else {
				target = Location::BuildElfLocation(GetFile(), wxString::Format("section/%i", header->sh_info));
				name = GetFile()->GetSectionName(header->sh_info);
			}
			mTable->SetCell(rowStart + 8, 0, "Info");
			mTable->SetCell(rowStart + 8, 1, name, target);

			mTable->SetCell(rowStart + 9, 0, "Address Alignment");
			mTable->SetCell(rowStart + 9, 1, wxString::Format("0x%x", header->sh_addralign));

			mTable->SetCell(rowStart + 10, 0, "Entry Size");
			mTable->SetCell(rowStart + 10, 1, wxString::Format("0x%x", header->sh_entsize));
		}
	} else {
		mTable->Setup(1, 2);
		mTable->SetColumnLabel(0, "Name");
		mTable->SetColumnLabel(1, "Value");

		mTable->SetCell(0, 0, "No Section Headers");
		mTable->SetCellSize(0, 0, 1, 2);
		wxFont font = mTable->GetCellFont(0, 0);
		font.SetStyle(wxFONTSTYLE_ITALIC);
		mTable->SetCellFont(0, 0, font);
	}

	mTable->AutoSizeColumns();

	return mTable;
}

void ViewSectionHeaders::doSetOffset(int offset)
{
}