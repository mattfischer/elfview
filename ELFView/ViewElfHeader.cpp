#include "ViewElfHeader.h"

ViewElfHeader::ViewElfHeader(wxWindow *parent, wxWindowID id)
: wxSimpleHtmlListBox(parent, id)
{
}

void ViewElfHeader::SetFile(ElfFile *file)
{
	mFile = file;

	Clear();

	const Elf32_Ehdr *header = mFile->GetHeader();

	Append(wxString::Format("Type: 0x%x", header->e_type));
	Append(wxString::Format("Machine: 0x%x", header->e_machine));
	Append(wxString::Format("Version: 0x%x", header->e_version));
	Append(wxString::Format("Entry point: 0x%08x", header->e_entry));
	Append(wxString::Format("Program Header Offset: 0x%x", header->e_phoff));
	Append(wxString::Format("Section Header Offset: 0x%x", header->e_shoff));
	Append(wxString::Format("Flags: 0x%x", header->e_flags));
	Append(wxString::Format("Header Size: 0x%x", header->e_ehsize));
	Append(wxString::Format("Program Header Entry Size: 0x%x", header->e_phentsize));
	Append(wxString::Format("Number of Program Headers: 0x%x", header->e_phnum));
	Append(wxString::Format("Section Header Entry Size: 0x%x", header->e_shentsize));
	Append(wxString::Format("Number of Section Headers: 0x%x", header->e_shnum));
	Append(wxString::Format("Program Header Entry Size: 0x%x", header->e_phentsize));
	Append(wxString::Format("Section Header String Table: 0x%x", header->e_shstrndx));
}
