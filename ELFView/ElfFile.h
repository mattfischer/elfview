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
	wxString GetSymbolName(Elf32_Word section, Elf32_Word symbol);
	wxString GetString(Elf32_Word stringTable, Elf32_Word offset);
	const Elf32_Dyn *GetDynamicEntry(Elf32_Sword tag);
	int GetContainingProgramHeader(Elf32_Addr addr);
	char *ReadSection(Elf32_Word section);

	int GetToken();

protected:
	wxFile mFile;

	Elf32_Ehdr mHeader;
	char *mSectionHeaders;
	char *mProgramHeaders;
	char *mDynamicSection;
	int mDynamicSize;

	std::map<int, char*> mStringTables;

	int mToken;
};

#endif