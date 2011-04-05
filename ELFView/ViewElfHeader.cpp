#include "ViewElfHeader.h"

#include <wx/htmllbox.h>

extern wxWindow *gWindowMain;
ViewElfHeader::ViewElfHeader(ElfFile *file, wxString location)
: View(file, location)
{
	SetName("ELF Header");
}

wxWindow *ViewElfHeader::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mHtmlListBox = new wxSimpleHtmlListBox(parent, id);
	
	wxArrayString arrayString;

	const Elf32_Ehdr *header = GetFile()->GetHeader();

	arrayString.Add(wxString::Format("Type: 0x%x", header->e_type));
	arrayString.Add(wxString::Format("Machine: 0x%x", header->e_machine));
	arrayString.Add(wxString::Format("Version: 0x%x", header->e_version));
	arrayString.Add(wxString::Format("Entry point: 0x%08x", header->e_entry));
	arrayString.Add(wxString::Format("Program Header Offset: 0x%x", header->e_phoff));
	arrayString.Add(wxString::Format("Section Header Offset: 0x%x", header->e_shoff));
	arrayString.Add(wxString::Format("Flags: 0x%x", header->e_flags));
	arrayString.Add(wxString::Format("Header Size: 0x%x", header->e_ehsize));
	arrayString.Add(wxString::Format("Program Header Entry Size: 0x%x", header->e_phentsize));
	arrayString.Add(wxString::Format("Number of Program Headers: 0x%x", header->e_phnum));
	arrayString.Add(wxString::Format("Section Header Entry Size: 0x%x", header->e_shentsize));
	arrayString.Add(wxString::Format("Number of Section Headers: 0x%x", header->e_shnum));
	arrayString.Add(wxString::Format("Program Header Entry Size: 0x%x", header->e_phentsize));
	arrayString.Add(wxString::Format("Section Header String Table: 0x%x", header->e_shstrndx));

	mHtmlListBox->Append(arrayString);
	
	return mHtmlListBox;
}