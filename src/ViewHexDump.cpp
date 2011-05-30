#include "ViewHexDump.h"

#include "Location.h"

#include <vector>
#include <algorithm>

ViewHexDump::ViewHexDump(ElfFile *file, wxString location)
: View(file, location)
{
	int base;

	if(Location::GetSectionString(location, 1) == "section") {
		mSection = Location::GetSectionInt(location, 2);
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
	} else if(Location::GetSectionString(location, 1) == "segment") {
		int segment = Location::GetSectionInt(location, 2);
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
	ElfFile *file = GetFile();
	mHtmlListBox = new wxSimpleHtmlListBox(parent, id);
	wxArrayString arrayString;

	std::vector<Symbol> symbols;
	char *relSectionBuf = NULL;
	int relSection;
	int relSymtab;

	const Elf32_Shdr *sectionHeader = file->GetSectionHeader(mSection);

	for(int i=0; i<file->GetHeader()->e_shnum; i++) {
		const Elf32_Shdr *header = file->GetSectionHeader(i);

		if(header->sh_type == SHT_REL || header->sh_type == SHT_RELA) {
			if(mSection != -1 && header->sh_info == mSection) {
				relSection = i;
				relSectionBuf = file->ReadSection(relSection);
				relSymtab = header->sh_link;
			}
			continue;
		}

		if(header->sh_type != SHT_SYMTAB && header->sh_type != SHT_DYNSYM) {
			continue;
		}

		char *buffer = file->ReadSection(i);

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
			if(ELF32_ST_TYPE(sym->st_info) == STT_SECTION) {
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

//	std::sort(symbols.begin(), symbols.end());

	unsigned char *buffer = new unsigned char[mFileSize];
	GetFile()->Read(buffer, mOffset, mFileSize);

	int addrsize = 1;
	int sz = (mSize + mBase) >> 4;
	while(sz > 0) {
		sz >>= 4;
		addrsize++;
	}

	int start = 0;
	int end;
	int relIndex = 0;
	for(int symbol = 0; symbol < symbols.size() + 1; symbol++) {
		if(symbol < symbols.size()) {
			end = symbols[symbol].offset;
		} else {
			end = mSize;
		}

		for(int i=start; i<end; i += 4) {
			wxString hex;
			wxString ascii;
			int extraSpace = 4;
			int num = 4;
			if(end - i < 4) {
				num = end - i;
			}

			for(int j=i + num - 1; j>=i; j--) {
				unsigned char c;
				
				if(j < mFileSize ) {
					c = buffer[j];
				} else {
					c = 0;
				}

				hex += wxString::Format("%02x", (int)c);

				extraSpace--;
			}

			for(int j=0; j<extraSpace; j++) {
				hex += "  ";
			}

			int a = (i + mBase) >> 4;
			int sz = 1;
			while(a > 0) {
				a >>= 4;
				sz++;
			}
			
			wxString addr;
			for(int x = 0; x < addrsize - sz; x++ ) {
				addr += " ";
			}

			if(mRel) {
				addr += wxString::Format("0x%x: ", i);
			} else {
				addr += wxString::Format("0x%08x: ", i + mBase);
			}

			mOffsets.push_back(i);

			wxString relString;
			if(relSectionBuf != NULL) {
				const Elf32_Shdr *relSectionHeader = file->GetSectionHeader(relSection);
				if(relIndex * relSectionHeader->sh_entsize < relSectionHeader->sh_size) {
					Elf32_Rel *rel = (Elf32_Rel*)(relSectionBuf + relSectionHeader->sh_entsize * relIndex);
					if(rel->r_offset == i) {
						wxString relText = file->GetSymbolName(relSymtab, ELF32_R_SYM(rel->r_info));
						wxString relTarget = Location::BuildElfLocation(file, wxString::Format("section/%i", relSection), relSectionHeader->sh_entsize * relIndex);
						relString = "<a href=\"" + relTarget + "\">" + relText + "</a>";
						relIndex++;
					}
				}
			}
			arrayString.Add("<pre>" + addr + hex + "  " + relString + "</pre>");
		}

		if(symbol < symbols.size()) {
			mOffsets.push_back(end);
			wxString target = Location::BuildElfLocation(file, wxString::Format("section/%i", symbols[symbol].section), symbols[symbol].num * file->GetSectionHeader(symbols[symbol].section)->sh_entsize);
			arrayString.Add("<pre><a href=\"" +target + "\">" + symbols[symbol].name + "</a>:</pre>");
			
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
			
void ViewHexDump::doSetOffset(int offset)
{
	for(int i=1; i<mOffsets.size(); i++) {
		if(mOffsets[i] > offset) {
			mHtmlListBox->SetSelection(i - 1);
			return;
		}
	}

	mHtmlListBox->SetSelection(mOffsets.size() - 1);
}
