#include "ViewProgramHeaders.h"

ViewProgramHeaders::ViewProgramHeaders()
{
	SetName("Program Headers");
}

wxWindow *ViewProgramHeaders::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mHtmlListBox = new wxSimpleHtmlListBox(parent, id);

	return mHtmlListBox;
}

void ViewProgramHeaders::doUpdateWindow()
{
	mHtmlListBox->Clear();

	if(GetFile()->GetProgramHeaders()) {
		for(int i=0; i<GetFile()->GetHeader()->e_phnum;i++) {
			const Elf32_Phdr *header = GetFile()->GetProgramHeaders() + i;

			mHtmlListBox->Append(wxString::Format("<b>Segment %i</b>", i));
			mHtmlListBox->Append(wxString::Format("Type: 0x%x", header->p_type));
			mHtmlListBox->Append(wxString::Format("Offset: 0x%x", header->p_offset));
			mHtmlListBox->Append(wxString::Format("VAddr: 0x%x", header->p_vaddr));
			mHtmlListBox->Append(wxString::Format("PAddr: 0x%x", header->p_paddr));
			mHtmlListBox->Append(wxString::Format("File Size: 0x%x", header->p_filesz));
			mHtmlListBox->Append(wxString::Format("Memory Size: 0x%x", header->p_memsz));
			mHtmlListBox->Append(wxString::Format("Flags: 0x%x", header->p_flags));
			mHtmlListBox->Append(wxString::Format("Alignment: 0x%x", header->p_align));
			mHtmlListBox->Append("");
		}
	} else {
		mHtmlListBox->Append("<i>No Program Headers</i>");
	}
}