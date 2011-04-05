#ifndef ELF_FILE_H
#define ELF_FILE_H

#include "Elf32.h"

#include <wx/file.h>

#include <map>

class ElfFile
{
public:
	ElfFile(wxString &filename);

	size_t Read(void *buffer, wxFileOffset offset, size_t size);

	const Elf32_Ehdr *GetHeader();
	const Elf32_Shdr *GetSectionHeader(Elf32_Word section);
	const Elf32_Phdr *GetProgramHeader(Elf32_Word segment);

	wxString GetSectionName(Elf32_Word section);
	wxString GetString(Elf32_Word stringTable, Elf32_Word offset);

protected:
	wxFile mFile;

	Elf32_Ehdr mHeader;
	char *mSectionHeaders;
	char *mProgramHeaders;

	std::map<int, char*> mStringTables;
};

#endif