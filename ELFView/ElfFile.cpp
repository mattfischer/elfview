#include "ElfFile.h"

ElfFile::ElfFile(wxString &filename)
: mFile(filename.c_str())
{
	Read(&mHeader, 0, sizeof(Elf32_Ehdr));

	if(mHeader.e_phoff > 0) {
		mProgramHeaders = new Elf32_Phdr[mHeader.e_phnum];
		Read(mProgramHeaders, mHeader.e_phoff, mHeader.e_phnum * mHeader.e_phentsize);
	} else {
		mProgramHeaders = NULL;
	}

	if(mHeader.e_shoff > 0) {
		mSectionHeaders = new Elf32_Shdr[mHeader.e_shnum];
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

const Elf32_Shdr *ElfFile::GetSectionHeaders()
{
	return mSectionHeaders;
}

const Elf32_Phdr *ElfFile::GetProgramHeaders()
{
	return mProgramHeaders;
}

wxString ElfFile::GetString(Elf32_Word stringTable, Elf32_Word offset)
{
	char *table;

	if(mStringTables.find(stringTable) != mStringTables.end()) {
		table = mStringTables[stringTable];
	} else {
		table = new char[mSectionHeaders[stringTable].sh_size];
		Read(table, mSectionHeaders[stringTable].sh_offset, mSectionHeaders[stringTable].sh_size);
		mStringTables[stringTable] = table;
	}

	return wxString(table + offset);
}