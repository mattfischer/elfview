#include "ViewElfHeader.h"

#include <wx/htmllbox.h>

ViewElfHeader::ViewElfHeader()
{
	SetName("ELF Header");
}

wxWindow *ViewElfHeader::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mHtmlListBox = new wxSimpleHtmlListBox(parent, id);

	return mHtmlListBox;
}

void ViewElfHeader::doUpdateWindow()
{
	mHtmlListBox->Clear();

	const Elf32_Ehdr *header = GetFile()->GetHeader();

	mHtmlListBox->Append(wxString::Format("Type: 0x%x", header->e_type));
	mHtmlListBox->Append(wxString::Format("Machine: 0x%x", header->e_machine));
	mHtmlListBox->Append(wxString::Format("Version: 0x%x", header->e_version));
	mHtmlListBox->Append(wxString::Format("Entry point: 0x%08x", header->e_entry));
	mHtmlListBox->Append(wxString::Format("Program Header Offset: 0x%x", header->e_phoff));
	mHtmlListBox->Append(wxString::Format("Section Header Offset: 0x%x", header->e_shoff));
	mHtmlListBox->Append(wxString::Format("Flags: 0x%x", header->e_flags));
	mHtmlListBox->Append(wxString::Format("Header Size: 0x%x", header->e_ehsize));
	mHtmlListBox->Append(wxString::Format("Program Header Entry Size: 0x%x", header->e_phentsize));
	mHtmlListBox->Append(wxString::Format("Number of Program Headers: 0x%x", header->e_phnum));
	mHtmlListBox->Append(wxString::Format("Section Header Entry Size: 0x%x", header->e_shentsize));
	mHtmlListBox->Append(wxString::Format("Number of Section Headers: 0x%x", header->e_shnum));
	mHtmlListBox->Append(wxString::Format("Program Header Entry Size: 0x%x", header->e_phentsize));
	mHtmlListBox->Append(wxString::Format("Section Header String Table: 0x%x", header->e_shstrndx));
}