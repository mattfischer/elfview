#include "ViewSectionHeaders.h"

ViewSectionHeaders::ViewSectionHeaders()
{
	SetName("Section Headers");
}

wxWindow *ViewSectionHeaders::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mHtmlListBox = new wxSimpleHtmlListBox(parent, id);

	return mHtmlListBox;
}

void ViewSectionHeaders::doUpdateWindow()
{
	mHtmlListBox->Clear();

	if(GetFile()->GetSectionHeaders()) {
		for(int i=0; i<GetFile()->GetHeader()->e_shnum;i++) {
			const Elf32_Shdr *header = GetFile()->GetSectionHeaders() + i;

			wxString name = GetFile()->GetString(GetFile()->GetHeader()->e_shstrndx, header->sh_name);

			if(name == "") {
				mHtmlListBox->Append(wxString::Format("<b>Section %i</b>", i));
			} else {
				mHtmlListBox->Append(wxString::Format("<b>Section %i (%s)</b>", i, name.c_str()));
			}

			mHtmlListBox->Append(wxString::Format("Name: %s", name.c_str()));
			mHtmlListBox->Append(wxString::Format("Type: 0x%x", header->sh_type));
			mHtmlListBox->Append(wxString::Format("Flags: 0x%x", header->sh_flags));
			mHtmlListBox->Append(wxString::Format("Address: 0x%x", header->sh_addr));
			mHtmlListBox->Append(wxString::Format("Offset: 0x%x", header->sh_offset));
			mHtmlListBox->Append(wxString::Format("Size: 0x%x", header->sh_size));
			mHtmlListBox->Append(wxString::Format("Link: 0x%x", header->sh_link));
			mHtmlListBox->Append(wxString::Format("Info: 0x%x", header->sh_info));
			mHtmlListBox->Append(wxString::Format("Address Alignment: 0x%x", header->sh_addralign));
			mHtmlListBox->Append(wxString::Format("Entry Size: 0x%x", header->sh_entsize));
			mHtmlListBox->Append("");
		}
	} else {
		mHtmlListBox->Append("<i>No Section Headers</i>");
	}
}