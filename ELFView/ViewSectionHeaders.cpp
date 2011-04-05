#include "ViewSectionHeaders.h"

extern wxWindow *gWindowMain;

ViewSectionHeaders::ViewSectionHeaders(ElfFile *file, wxString location)
: View(file, location)
{
	SetName("Section Headers");
}

wxWindow *ViewSectionHeaders::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mHtmlListBox = new wxSimpleHtmlListBox(parent, id);

	wxArrayString arrayString;

	if(GetFile()->GetSectionHeaders()) {
		for(int i=0; i<GetFile()->GetHeader()->e_shnum;i++) {
			const Elf32_Shdr *header = GetFile()->GetSectionHeaders() + i;

			wxString name = GetFile()->GetString(GetFile()->GetHeader()->e_shstrndx, header->sh_name);

			if(name == "") {
				arrayString.Add(wxString::Format("<b>Section %i</b>", i));
			} else {
				arrayString.Add(wxString::Format("<b>Section %i (%s)</b>", i, name.c_str()));
			}

			arrayString.Add(wxString::Format("Name: %s", name.c_str()));
			arrayString.Add(wxString::Format("Type: 0x%x", header->sh_type));
			arrayString.Add(wxString::Format("Flags: 0x%x", header->sh_flags));
			arrayString.Add(wxString::Format("Address: 0x%x", header->sh_addr));
			arrayString.Add(wxString::Format("Offset: 0x%x", header->sh_offset));
			arrayString.Add(wxString::Format("Size: 0x%x", header->sh_size));
			arrayString.Add(wxString::Format("Link: 0x%x", header->sh_link));
			arrayString.Add(wxString::Format("Info: 0x%x", header->sh_info));
			arrayString.Add(wxString::Format("Address Alignment: 0x%x", header->sh_addralign));
			arrayString.Add(wxString::Format("Entry Size: 0x%x", header->sh_entsize));
			arrayString.Add("");
		}
	} else {
		arrayString.Add("<i>No Section Headers</i>");
	}

	mHtmlListBox->Append(arrayString);

	return mHtmlListBox;
}