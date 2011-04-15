#include "ViewProgramHeaders.h"

extern wxWindow *gWindowMain;

ViewProgramHeaders::ViewProgramHeaders(ElfFile *file, wxString location)
: View(file, location)
{
	SetName("Program Headers");
}

wxWindow *ViewProgramHeaders::doCreateWindow(wxWindow *parent, wxWindowID id)
{
	mHtmlListBox = new wxSimpleHtmlListBox(parent, id);

	wxArrayString arrayString;

	if(GetFile()->GetHeader()->e_phnum > 0) {
		for(int i=0; i<GetFile()->GetHeader()->e_phnum;i++) {
			const Elf32_Phdr *header = GetFile()->GetProgramHeader(i);

			arrayString.Add(wxString::Format("<b>Segment %i</b>", i));
			arrayString.Add(wxString::Format("Type: 0x%x", header->p_type));
			arrayString.Add(wxString::Format("Offset: 0x%x", header->p_offset));
			arrayString.Add(wxString::Format("VAddr: 0x%x", header->p_vaddr));
			arrayString.Add(wxString::Format("PAddr: 0x%x", header->p_paddr));
			arrayString.Add(wxString::Format("File Size: 0x%x", header->p_filesz));
			arrayString.Add(wxString::Format("Memory Size: 0x%x", header->p_memsz));
			arrayString.Add(wxString::Format("Flags: 0x%x", header->p_flags));
			arrayString.Add(wxString::Format("Alignment: 0x%x", header->p_align));
			arrayString.Add("");
		}
	} else {
		arrayString.Add("<i>No Program Headers</i>");
	}

	mHtmlListBox->Append(arrayString);

	return mHtmlListBox;
}

void ViewProgramHeaders::doSetOffset(int offset)
{
}