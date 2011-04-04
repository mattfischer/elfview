#include "ViewSectionHeaders.h"

ViewSectionHeaders::ViewSectionHeaders(wxWindow *parent, wxWindowID id)
: wxSimpleHtmlListBox(parent, id)
{
}

void ViewSectionHeaders::SetFile(ElfFile *file)
{
	mFile = file;

	Clear();

	if(mFile->GetSectionHeaders()) {
		for(int i=0; i<mFile->GetHeader()->e_shnum;i++) {
			const Elf32_Shdr *header = mFile->GetSectionHeaders() + i;

			Append(wxString::Format("<b>Section %i</b>", i));
			Append(wxString::Format("Name: %s", mFile->GetString(mFile->GetHeader()->e_shstrndx, header->sh_name).c_str()));
			Append(wxString::Format("Type: 0x%x", header->sh_type));
			Append(wxString::Format("Flags: 0x%x", header->sh_flags));
			Append(wxString::Format("Address: 0x%x", header->sh_addr));
			Append(wxString::Format("Offset: 0x%x", header->sh_offset));
			Append(wxString::Format("Size: 0x%x", header->sh_size));
			Append(wxString::Format("Link: 0x%x", header->sh_link));
			Append(wxString::Format("Info: 0x%x", header->sh_info));
			Append(wxString::Format("Address Alignment: 0x%x", header->sh_addralign));
			Append(wxString::Format("Entry Size: 0x%x", header->sh_entsize));
			Append("");
		}
	} else {
		Append("<i>No Section Headers</i>");
	}
}