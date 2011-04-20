#include "ElfFile.h"

static int nextToken = 0;

ElfFile::ElfFile(wxString &filename)
: mFile(filename.c_str())
{
	Read(&mHeader, 0, sizeof(Elf32_Ehdr));

	if(mHeader.e_phoff > 0) {
		mProgramHeaders = new char[mHeader.e_phnum * mHeader.e_phentsize];
		Read(mProgramHeaders, mHeader.e_phoff, mHeader.e_phnum * mHeader.e_phentsize);
	} else {
		mProgramHeaders = NULL;
	}

	if(mHeader.e_shoff > 0) {
		mSectionHeaders = new char[mHeader.e_shnum * mHeader.e_shentsize];
		Read(mSectionHeaders, mHeader.e_shoff, mHeader.e_shnum * mHeader.e_shentsize);
	} else {
		mSectionHeaders = NULL;
	}

	mDynamicSection = NULL;
	mDynamicSize = 0;
	if(mHeader.e_phnum > 0) {
		for(int i=0; i<mHeader.e_phnum; i++) {
			const Elf32_Phdr *header = GetProgramHeader(i);

			if(header->p_type == PT_DYNAMIC) {
				mDynamicSection = new char[header->p_filesz];
				mDynamicSize = header->p_filesz;
				Read(mDynamicSection, header->p_offset, header->p_filesz);
				break;
			}
		}
	}

	mToken = nextToken++;
}

size_t ElfFile::Read(void *buffer, wxFileOffset offset, size_t size)
{
	mFile.Seek(offset);

	return mFile.Read(buffer, size);
}

const Elf32_Ehdr *ElfFile::GetHeader()
{
	return &mHeader;
}

const Elf32_Shdr *ElfFile::GetSectionHeader(Elf32_Word section)
{
	return (Elf32_Shdr*)(mSectionHeaders + mHeader.e_shentsize * section);
}

const Elf32_Phdr *ElfFile::GetProgramHeader(Elf32_Word segment)
{
	return (Elf32_Phdr*)(mProgramHeaders + mHeader.e_phentsize * segment);
}

wxString ElfFile::GetString(Elf32_Word stringTable, Elf32_Word offset)
{
	char *table;

	if(mStringTables.find(stringTable) != mStringTables.end()) {
		table = mStringTables[stringTable];
	} else {
		const Elf32_Shdr *section = GetSectionHeader(stringTable);
		table = new char[section->sh_size];
		Read(table, section->sh_offset, section->sh_size);
		mStringTables[stringTable] = table;
	}

	return wxString(table + offset);
}

wxString ElfFile::GetSectionName(Elf32_Word section)
{
	wxString name = GetString(mHeader.e_shstrndx, GetSectionHeader(section)->sh_name);

	if(name == "") {
		return wxString::Format("%i", section);
	} else {
		return name;
	}
}

wxString ElfFile::GetSymbolName(Elf32_Word section, Elf32_Word symbol)
{
	Elf32_Sym sym;

	const Elf32_Shdr *header = GetSectionHeader(section);
	Read(&sym, header->sh_offset + symbol * header->sh_entsize, header->sh_entsize);

	return GetString(header->sh_link, sym.st_name);
}

const Elf32_Dyn *ElfFile::GetDynamicEntry(Elf32_Sword tag)
{
	if(mDynamicSection == NULL) {
		return NULL;
	}

	for(int i=0; i<mDynamicSize / sizeof(Elf32_Dyn); i++) {
		Elf32_Dyn *dyn = (Elf32_Dyn*)(mDynamicSection + i * sizeof(Elf32_Dyn));

		if(dyn->d_tag == tag) {
			return dyn;
		}
	}

	return NULL;
}

int ElfFile::GetContainingProgramHeader(Elf32_Addr addr)
{
	for(int i=0; i<mHeader.e_phnum; i++) {
		const Elf32_Phdr *header = GetProgramHeader(i);

		if(addr >= header->p_vaddr && addr < header->p_vaddr + header->p_memsz) {
			return i;
		}
	}

	return 0;
}

char *ElfFile::ReadSection(Elf32_Word section)
{
	const Elf32_Shdr *header = GetSectionHeader(section);

	char *buffer = new char[header->sh_size];
	Read(buffer, header->sh_offset, header->sh_size);

	return buffer;
}

int ElfFile::GetToken()
{
	return mToken;
}
