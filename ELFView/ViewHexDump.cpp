#include "ViewHexDump.h"

#include "Location.h"

#include <vector>
#include <algorithm>

ViewHexDump::ViewHexDump(ElfFile *file, wxString location)
: View(file, location)
{
	int base;

	if(Location::GetSectionString(location, 0) == "section") {
		mSection = Location::GetSectionInt(location, 1);
		const Elf32_Shdr *header = GetFile()->GetSectionHeader(mSection);
		mOffset = header->sh_offset;
		mSize = header->sh_size;
		
		if(header->sh_type == SHT_NOBITS) {
			mFileSize = 0;
		} else {
			mFileSize = mSize;
		}

		base = header->sh_addr;
		
		SetName(file->GetSectionName(mSection));
	} else if(Location::GetSectionString(location, 0) == "segment") {
		int segment = Location::GetSectionInt(location, 1);
		const Elf32_Phdr *header = GetFile()->GetProgramHeader(segment);
		mOffset = header->p_offset;
		mSize = header->p_memsz;
		mFileSize = header->p_filesz;
		base = header->p_vaddr;
		mSection = -1;

		SetName(wxString::Format("Program Header %i", segment));
	}

	const Elf32_Ehdr *header = GetFile()->GetHeader();

	if(header->e_type == ET_REL) {
		mBase = 0;
		mRel = true;
	} else {
		mBase = base;
		mRel = false;
	}
}

struct Symbol {
	int offset;
	int section;
	int num;
	wxString name;

	int operator<(const Symbol &other) {
		if(offset == other.offset) {
			return name < other.name;
		} else {
			return offset < other.offset;
		}
	}
};

wxWindow *ViewHexDump::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mHtmlListBox = new wxSimpleHtmlListBox(parent, id);
	wxArrayString arrayString;

	std::vector<Symbol> symbols;

	const Elf32_Shdr *sectionHeader = GetFile()->GetSectionHeader(mSection);

	for(int i=0; i<GetFile()->GetHeader()->e_shnum; i++) {
		const Elf32_Shdr *header = GetFile()->GetSectionHeader(i);

		if(header->sh_type != SHT_SYMTAB && header->sh_type != SHT_DYNSYM) {
			continue;
		}

		char *buffer = new char[header->sh_size];
		GetFile()->Read(buffer, header->sh_offset, header->sh_size);

		for(int j=0; j<header->sh_size / header->sh_entsize; j++) {
			Elf32_Sym *sym = (Elf32_Sym*) (buffer + j * header->sh_entsize);

			if(mRel && mSection != -1) {
				if(sym->st_shndx != mSection) {
					continue;
				}
			} else {
				if(sym->st_value < mBase || sym->st_value >= mBase + mSize) {
					continue;
				}
			}
			if(ELF32_ST_TYPE(sym->st_info) != STT_OBJECT && ELF32_ST_TYPE(sym->st_info) != STT_FUNC) {
				continue;
			}

			Symbol s;

			s.offset = sym->st_value - mBase;
			s.section = i;
			s.num = j;
			s.name = GetFile()->GetString(header->sh_link, sym->st_name);

			symbols.push_back(s);
		}

		delete[] buffer;
	}

	std::sort(symbols.begin(), symbols.end());

	unsigned char *buffer = new unsigned char[mFileSize];
	GetFile()->Read(buffer, mOffset, mFileSize);

	int start = 0;
	int end;
	for(int symbol = 0; symbol < symbols.size() + 1; symbol++) {
		if(symbol < symbols.size()) {
			end = symbols[symbol].offset;
		} else {
			end = mSize;
		}

		for(int i=start; i<end; i += 8) {
			wxString hex;
			wxString ascii;
			int extraSpace = 8;

			for(int j=i; j<i+8 && j<end; j++) {
				unsigned char c;
				
				if(j < mFileSize ) {
					c = buffer[j];
				} else {
					c = 0;
				}

				hex += wxString::Format("0x%02x ", (int)c);

				if(c == '>') {
					ascii += "&gt;";
				} else if(c == '<') {
					ascii += "&lt;";
				} else if(c == ' ') {
					ascii += "&nbsp;";
				} else if(c == '&') {
					ascii += "&amp;";
				} else if(c > ' ' && c < 127) {
					ascii += c;
				} else {
					ascii += '.';
				}

				extraSpace--;
			}

			for(int j=0; j<extraSpace; j++) {
				hex += "     ";
			}

			wxString addr;
			if(mRel) {
				addr = wxString::Format("0x%x: ", i);
			} else {
				addr = wxString::Format("0x%08x: ", i + mBase);
			}

			arrayString.Add("<pre>" + addr + hex + "        " + ascii + "</pre>");
		}

		if(symbol < symbols.size()) {
			arrayString.Add("<pre>" + symbols[symbol].name + ":</pre>");

			int newsym = symbol;
			while(newsym < symbols.size() - 1 &&
				symbols[newsym + 1].offset == symbols[symbol].offset &&
				symbols[newsym + 1].name == symbols[symbol].name) newsym++;

			symbol = newsym;
			start = end;
		}
	}

	delete[] buffer;

	mHtmlListBox->Append(arrayString);

	return mHtmlListBox;
}
			