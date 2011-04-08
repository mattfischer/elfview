#include "ViewHexDump.h"

#include "Util.h"

#include <vector>

ViewHexDump::ViewHexDump(ElfFile *file, wxString location)
: View(file, location)
{
	mSection = Util::GetSectionNumber(location);
	SetName(file->GetSectionName(mSection));
}

struct Symbol {
	int offset;
	int section;
	int num;
};

wxWindow *ViewHexDump::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mHtmlListBox = new wxSimpleHtmlListBox(parent, id);

	std::vector<Symbol> symbols;

	for(int i=0; i<GetFile()->GetHeader()->e_shnum; i++) {
		const Elf32_Shdr *header = GetFile()->GetSectionHeader(i);

		if(header->sh_type != SHT_SYMTAB && header->sh_type != SHT_DYNSYM) {
			continue;
		}

		char *buffer = new char[header->sh_size];
		GetFile()->Read(buffer, header->sh_offset, header->sh_size);

		for(int j=0; j<header->sh_size / header->sh_entsize; j++) {
			Elf32_Sym *sym = (Elf32_Sym*) (buffer + j * header->sh_entsize);

			if(sym->st_shndx != mSection) {
				continue;
			}

			if(ELF32_ST_TYPE(sym->st_info) != STT_OBJECT && ELF32_ST_TYPE(sym->st_info) != STT_FUNC) {
				continue;
			}

			Symbol s;

			s.offset = sym->st_value;
			s.section = i;
			s.num = j;

			symbols.push_back(s);
		}

		delete[] buffer;
	}

	const Elf32_Shdr *header = GetFile()->GetSectionHeader(mSection);
	char *buffer = new char[header->sh_size];
	GetFile()->Read(buffer, header->sh_offset, header->sh_size);

	int start = 0;
	int end;
	for(int symbol = 0; symbol < symbols.size() + 1; symbol++) {
		if(symbol < symbols.size()) {
			end = symbols[symbol].offset;
		} else {
			end = header->sh_size;
		}

		for(int i=start; i<end; i += 8) {
			wxString hex;
			wxString ascii;
			int extraSpace = 8;

			for(int j=i; j<i+8 && j<end; j++) {
				unsigned char c = buffer[j];

				hex += wxString::Format("0x%02x ", (int)c);

				if(c >= ' ' && c < 127) {
					ascii += c;
				} else {
					ascii += '.';
				}

				extraSpace--;
			}

			for(int j=0; j<extraSpace; j++) {
				hex += "     ";
			}

			mHtmlListBox->Append("<pre>" + hex + "        " + ascii + "</pre>");
		}

		if(symbol < symbols.size()) {
			Elf32_Sym sym;
			const Elf32_Shdr *header = GetFile()->GetSectionHeader(symbols[symbol].section);
			GetFile()->Read(&sym, header->sh_offset + symbols[symbol].num * header->sh_entsize, header->sh_entsize);

			wxString name = GetFile()->GetString(header->sh_link, sym.st_name);
			mHtmlListBox->Append("<pre>" + name + ":</pre>");
		}

		start = end;
	}

	delete[] buffer;

	return mHtmlListBox;
}
			