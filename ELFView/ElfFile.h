#ifndef ELF_FILE_H
#define ELF_FILE_H

#include "Elf32.h"

#include <wx/file.h>

class ElfFile
{
public:
	ElfFile(wxString &filename);

	size_t Read(void *buffer, wxFileOffset offset, size_t size);

	const Elf32_Ehdr *GetHeader();
	const Elf32_Shdr *GetSectionHeaders();
	const Elf32_Phdr *GetProgramHeaders();

	wxString GetString(Elf32_Word stringTable, Elf32_Word offset);

protected:
	wxFile mFile;

	Elf32_Ehdr mHeader;
	Elf32_Shdr *mSectionHeaders;
	Elf32_Phdr *mProgramHeaders;
};

#endif