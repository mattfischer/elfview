#include "ElfFile.h"

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
	return GetString(mHeader.e_shstrndx, GetSectionHeader(section)->sh_name);
}